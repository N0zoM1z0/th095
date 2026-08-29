#!/usr/bin/env bash
set -euo pipefail

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
tools_dir="$repo_root/.tools"
downloads="$tools_dir/downloads"
ghidra_version_dir="$tools_dir/ghidra_12.1.3_PUBLIC"
jdk_version_dir="$tools_dir/jdk-21.0.12.1+1"
msvc_root="$tools_dir/msvc710"
objdiff_root="$tools_dir/objdiff"

ghidra_asset="ghidra_12.1.3_PUBLIC_20260817.zip"
ghidra_url="https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.3_build/$ghidra_asset"
ghidra_sha256="93a5d11a9ad510622acaaf908c556a7b9b764d338e78a7567f3689bf5081fd54"
jdk_asset="OpenJDK21U-jdk_x64_linux_hotspot_21.0.12.1_1.tar.gz"
jdk_url="https://github.com/adoptium/temurin21-binaries/releases/download/jdk-21.0.12.1%2B1/$jdk_asset"
jdk_sha256="ce79869e1307ed8ee1e2baa86a412b1eb5b75d10a01006d788a6f968bcfaee94"
msvc_commit="2932d76fe417b0bc49010b26d4be2e5b743cc4be"
msvc_cl_sha256="2ecf86a3edfd3deae498e08298e210e984537ce9e11759930561e43f40bd2515"
msvc_link_sha256="0d5f9712d0da843d787bf4ce4e678abc6b737940996706b2c86101eac9df3e3c"
objdiff_url="https://github.com/encounter/objdiff/releases/download/v3.8.0/objdiff-cli-linux-x86_64"
objdiff_sha256="bc1e047126f9c6914bd1695798175234642ab9eaf45e886f841b59a4231e1a81"

for command_name in curl git sha256sum tar unzip uv wine; do
  if ! command -v "$command_name" >/dev/null; then
    echo "missing prerequisite: $command_name" >&2
    exit 1
  fi
done

mkdir -p "$downloads" "$tools_dir"

download_checked() {
  local download_url=$1
  local output_path=$2
  local expected_hash=$3
  if [[ ! -f "$output_path" ]]; then
    curl --fail --location --retry 3 --output "$output_path" "$download_url"
  fi
  echo "$expected_hash  $output_path" | sha256sum --check --status || {
    echo "checksum mismatch: $output_path" >&2
    exit 1
  }
}

download_checked "$ghidra_url" "$downloads/$ghidra_asset" "$ghidra_sha256"
if [[ ! -d "$ghidra_version_dir" ]]; then
  unzip -q "$downloads/$ghidra_asset" -d "$tools_dir"
fi
ln -sfn "$(basename "$ghidra_version_dir")" "$tools_dir/ghidra"

download_checked "$jdk_url" "$downloads/$jdk_asset" "$jdk_sha256"
if [[ ! -d "$jdk_version_dir" ]]; then
  tar -xzf "$downloads/$jdk_asset" -C "$tools_dir"
fi
ln -sfn "$(basename "$jdk_version_dir")" "$tools_dir/jdk"

if [[ ! -d "$msvc_root/.git" ]]; then
  git clone --filter=blob:none https://github.com/archaic-msvc/msvc710.git "$msvc_root"
fi
git -C "$msvc_root" fetch --quiet origin "$msvc_commit"
git -C "$msvc_root" checkout --quiet --detach "$msvc_commit"
echo "$msvc_cl_sha256  $msvc_root/Vc7/bin/cl.exe" | sha256sum --check --status
echo "$msvc_link_sha256  $msvc_root/Vc7/bin/link.exe" | sha256sum --check --status

if [[ "$(uname -m)" != "x86_64" ]]; then
  echo "the pinned objdiff CLI asset supports Linux x86_64 only" >&2
  exit 1
fi
if [[ ! -x "$objdiff_root/objdiff-cli" ]]; then
  objdiff_asset="$downloads/objdiff-cli-linux-x86_64-v3.8.0"
  download_checked "$objdiff_url" "$objdiff_asset" "$objdiff_sha256"
  mkdir -p "$objdiff_root"
  install -m 0755 "$objdiff_asset" "$objdiff_root/objdiff-cli"
fi

if ! command -v reccmp-project >/dev/null || \
  [[ "$(reccmp-project --version 2>/dev/null || true)" != *"0.1.6"* ]]; then
  uv tool install --force reccmp==0.1.6
fi

ghidra_usage=$(JAVA_HOME="$tools_dir/jdk" \
  "$tools_dir/ghidra/support/analyzeHeadless" 2>&1 || true)
if [[ "$ghidra_usage" != *"Headless Analyzer Usage"* ]]; then
  echo "Ghidra headless smoke check failed" >&2
  exit 1
fi
compiler_usage=$(WINEDEBUG=-all wine "$msvc_root/Vc7/bin/cl.exe" 2>&1 || true)
if [[ "$compiler_usage" != *"Compiler Version 13.10.3077"* ]]; then
  echo "VC7.1 compiler smoke check failed" >&2
  exit 1
fi

echo "TH095 tool bootstrap complete"
echo "source scripts/tool-env.sh before interactive Ghidra or VC7.1 work"
