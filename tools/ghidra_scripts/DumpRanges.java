// Headless Ghidra script: print "addr  bytes  mnemonic operands" for address ranges.
// Args: out-file, then one or more ranges as start-end (hex, inclusive-exclusive), e.g. 8c0230b0-8c0231d2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import java.io.PrintWriter;

public class DumpRanges extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        PrintWriter out = new PrintWriter(args[0]);
        Listing listing = currentProgram.getListing();
        for (int i = 1; i < args.length; i++) {
            String[] se = args[i].split("-");
            long start = Long.parseLong(se[0], 16), end = Long.parseLong(se[1], 16);
            out.println("=== " + args[i] + " ===");
            Address a = toAddr(start);
            while (a.getOffset() < end) {
                Instruction ins = listing.getInstructionAt(a);
                if (ins == null) {
                    // force-disassemble if analysis treated it as data
                    disassemble(a);
                    ins = listing.getInstructionAt(a);
                }
                if (ins == null) {
                    byte[] b = new byte[2];
                    currentProgram.getMemory().getBytes(a, b);
                    out.printf("%08x  %02x%02x  ??%n", a.getOffset(), b[0] & 0xff, b[1] & 0xff);
                    a = a.add(2);
                    continue;
                }
                byte[] b = ins.getBytes();
                StringBuilder hex = new StringBuilder();
                for (byte x : b) hex.append(String.format("%02x", x & 0xff));
                out.printf("%08x  %-6s %s%n", a.getOffset(), hex, ins.toString());
                a = a.add(ins.getLength());
            }
        }
        out.close();
    }
}
