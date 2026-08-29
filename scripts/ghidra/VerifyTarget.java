// Fail-closed attestation for a loaded TH095 program.
//@author N0zoM1z0
//@category TH095

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class VerifyTarget extends GhidraScript
{
    @Override
    protected void run() throws Exception
    {
        String[] args = getScriptArgs();
        if (args.length != 4)
            throw new IllegalArgumentException(
                "usage: VerifyTarget.java SHA256 IMAGE_BASE ENTRY_POINT TEXT_START");

        String expectedHash = args[0].toLowerCase();
        long expectedBase = Long.decode(args[1]);
        Address expectedEntry = toAddr(args[2]);
        Address expectedText = toAddr(args[3]);
        String actualHash = currentProgram.getExecutableSHA256();

        if (actualHash == null || !actualHash.toLowerCase().equals(expectedHash))
            throw new IllegalStateException(
                "program SHA-256 mismatch: " + actualHash + " != " + expectedHash);
        if (currentProgram.getImageBase().getOffset() != expectedBase)
            throw new IllegalStateException(
                "image base mismatch: " + currentProgram.getImageBase());
        if (!currentProgram.getMemory().contains(expectedText))
            throw new IllegalStateException("target .text start is not mapped");
        Function entry = getFunctionContaining(expectedEntry);
        if (entry == null)
            throw new IllegalStateException(
                "no analyzed function contains expected entry point " + expectedEntry);

        println("TH095 Ghidra target attested: " + actualHash);
        println("image base=" + currentProgram.getImageBase() +
                " entry=" + expectedEntry + " function=" + entry.getName(true));
    }
}
