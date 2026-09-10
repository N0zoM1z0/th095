param(
    [string]$Executable = ""
)

$ErrorActionPreference = "Stop"

# This launcher is intentionally pinned to one reconstructed artifact.  The
# original Japanese executable must never be patched, and a rebuilt image may
# move the function even when its source is unchanged.  Refuse every unknown
# hash instead of guessing an address.
$ExpectedSha256 = "8e009628f6e41af753b0eb765877c41b877d9f412b020f1b3607cfdbdcfac97f"
$DieRva = 0x000342F0
$ExpectedPrologue = [byte[]]@(0x55, 0x8B, 0xEC)
$ReturnInstruction = [byte[]]@(0xC3)

if ([string]::IsNullOrWhiteSpace($Executable)) {
    $DeployedExecutable = Join-Path $PSScriptRoot "th095-reconstructed.exe"
    $RepositoryExecutable = Join-Path $PSScriptRoot "..\build\whole-validation\th095-reconstructed.exe"
    if (Test-Path -LiteralPath $DeployedExecutable -PathType Leaf) {
        $Executable = $DeployedExecutable
    }
    else {
        $Executable = $RepositoryExecutable
    }
}
$Executable = [System.IO.Path]::GetFullPath($Executable)

$NativeSource = @'
using System;
using System.Runtime.InteropServices;

public static class Th095RuntimePatchNative
{
    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool ReadProcessMemory(
        IntPtr process, IntPtr address, [Out] byte[] buffer,
        UIntPtr size, out UIntPtr bytesRead);

    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool WriteProcessMemory(
        IntPtr process, IntPtr address, byte[] buffer,
        UIntPtr size, out UIntPtr bytesWritten);

    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool VirtualProtectEx(
        IntPtr process, IntPtr address, UIntPtr size,
        uint newProtection, out uint oldProtection);

    [DllImport("kernel32.dll", SetLastError = true)]
    public static extern bool FlushInstructionCache(
        IntPtr process, IntPtr address, UIntPtr size);
}
'@

function Throw-LastWin32Error([string]$Operation) {
    $Code = [Runtime.InteropServices.Marshal]::GetLastWin32Error()
    throw "$Operation failed with Win32 error $Code"
}

$GameProcess = $null
try {
    if (-not (Test-Path -LiteralPath $Executable -PathType Leaf)) {
        throw "Missing reconstructed executable: $Executable"
    }
    $ActualSha256 = (Get-FileHash -LiteralPath $Executable -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($ActualSha256 -ne $ExpectedSha256) {
        throw (
            "Unsupported executable hash: $ActualSha256. " +
            "This launcher accepts only reconstructed build $ExpectedSha256."
        )
    }

    Add-Type -TypeDefinition $NativeSource -Language CSharp
    $StartOptions = @{
        FilePath = $Executable
        WorkingDirectory = [System.IO.Path]::GetDirectoryName($Executable)
        PassThru = $true
    }
    $GameProcess = Start-Process @StartOptions

    $Deadline = [DateTime]::UtcNow.AddSeconds(10)
    $ModuleBase = [IntPtr]::Zero
    while ([DateTime]::UtcNow -lt $Deadline) {
        if ($GameProcess.HasExited) {
            throw "The reconstructed game exited before the runtime patch was installed."
        }
        try {
            $GameProcess.Refresh()
            $ModuleBase = $GameProcess.MainModule.BaseAddress
            if ($ModuleBase -ne [IntPtr]::Zero) {
                break
            }
        }
        catch {
            # The loader can briefly make MainModule unavailable immediately
            # after Start-Process. Retry within the bounded startup window.
        }
        Start-Sleep -Milliseconds 50
    }
    if ($ModuleBase -eq [IntPtr]::Zero) {
        throw "Timed out while locating the reconstructed executable in memory."
    }

    $PatchAddress = [IntPtr]($ModuleBase.ToInt64() + $DieRva)
    $Before = New-Object byte[] $ExpectedPrologue.Length
    $BeforeSize = [UIntPtr]::new([uint64]$Before.Length)
    $PatchSize = [UIntPtr]::new([uint64]$ReturnInstruction.Length)
    $BytesRead = [UIntPtr]::Zero
    if (-not [Th095RuntimePatchNative]::ReadProcessMemory(
        $GameProcess.Handle,
        $PatchAddress,
        $Before,
        $BeforeSize,
        [ref]$BytesRead
    )) {
        Throw-LastWin32Error "ReadProcessMemory"
    }
    if ($BytesRead.ToUInt64() -ne $Before.Length) {
        throw "ReadProcessMemory returned an incomplete death-function prologue."
    }
    if ([BitConverter]::ToString($Before) -ne [BitConverter]::ToString($ExpectedPrologue)) {
        throw (
            "Unexpected bytes at PhotoPlayerRuntimeView::Die: " +
            [BitConverter]::ToString($Before)
        )
    }

    # PAGE_EXECUTE_READWRITE is used only for this single byte and restored
    # immediately.  Replacing the entry PUSH EBP with RET makes every observed
    # bullet/laser death path return before it changes player mode, spawns death
    # effects, slows the game, or advances the failure state.
    $OldProtection = 0
    if (-not [Th095RuntimePatchNative]::VirtualProtectEx(
        $GameProcess.Handle,
        $PatchAddress,
        $PatchSize,
        0x40,
        [ref]$OldProtection
    )) {
        Throw-LastWin32Error "VirtualProtectEx(enable write)"
    }
    try {
        $BytesWritten = [UIntPtr]::Zero
        if (-not [Th095RuntimePatchNative]::WriteProcessMemory(
            $GameProcess.Handle,
            $PatchAddress,
            $ReturnInstruction,
            $PatchSize,
            [ref]$BytesWritten
        )) {
            Throw-LastWin32Error "WriteProcessMemory"
        }
        if ($BytesWritten.ToUInt64() -ne $ReturnInstruction.Length) {
            throw "WriteProcessMemory did not install the complete runtime patch."
        }
        if (-not [Th095RuntimePatchNative]::FlushInstructionCache(
            $GameProcess.Handle,
            $PatchAddress,
            $PatchSize
        )) {
            Throw-LastWin32Error "FlushInstructionCache"
        }
        $After = New-Object byte[] $ReturnInstruction.Length
        $BytesRead = [UIntPtr]::Zero
        if (-not [Th095RuntimePatchNative]::ReadProcessMemory(
            $GameProcess.Handle,
            $PatchAddress,
            $After,
            $PatchSize,
            [ref]$BytesRead
        )) {
            Throw-LastWin32Error "ReadProcessMemory(verify patch)"
        }
        if (
            $BytesRead.ToUInt64() -ne $ReturnInstruction.Length -or
            [BitConverter]::ToString($After) -ne [BitConverter]::ToString($ReturnInstruction)
        ) {
            throw "The runtime no-death patch failed its read-back check."
        }
    }
    finally {
        $IgnoredProtection = 0
        if (-not [Th095RuntimePatchNative]::VirtualProtectEx(
            $GameProcess.Handle,
            $PatchAddress,
            $PatchSize,
            $OldProtection,
            [ref]$IgnoredProtection
        )) {
            Throw-LastWin32Error "VirtualProtectEx(restore protection)"
        }
    }

    Write-Host "TH095 no-death test patch installed in process $($GameProcess.Id)." -ForegroundColor Green
    Write-Host "The executable on disk was not modified. Close the game normally when testing is complete."
    exit 0
}
catch {
    if ($null -ne $GameProcess -and -not $GameProcess.HasExited) {
        Stop-Process -Id $GameProcess.Id -Force -ErrorAction SilentlyContinue
    }
    Write-Error $_ -ErrorAction Continue
    exit 1
}
