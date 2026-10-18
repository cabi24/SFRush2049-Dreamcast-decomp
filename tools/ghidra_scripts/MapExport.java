// Headless Ghidra script (read-only): export functions, instruction coverage, references and defined data
// for the unit map. Arg: output directory. Writes ghidra_functions.tsv, ghidra_code.tsv, ghidra_refs.tsv,
// ghidra_data.tsv.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.PrintWriter;
import java.io.File;

public class MapExport extends GhidraScript {
    @Override
    public void run() throws Exception {
        String dir = getScriptArgs()[0];
        Listing listing = currentProgram.getListing();
        PrintWriter f = new PrintWriter(new File(dir, "ghidra_functions.tsv"));
        f.println("entry\tname\tbody");
        for (Function fn : listing.getFunctions(true)) {
            StringBuilder sb = new StringBuilder();
            for (AddressRange r : fn.getBody()) {
                if (sb.length() > 0) sb.append(",");
                sb.append(String.format("%08x-%08x", r.getMinAddress().getOffset(), r.getMaxAddress().getOffset() + 1));
            }
            f.printf("%08x\t%s\t%s%n", fn.getEntryPoint().getOffset(), fn.getName(), sb);
        }
        f.close();
        PrintWriter c = new PrintWriter(new File(dir, "ghidra_code.tsv"));
        long rs = -1, re = -1;
        InstructionIterator it = listing.getInstructions(true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            long a = ins.getAddress().getOffset();
            if (a != re) {
                if (rs >= 0) c.printf("%08x\t%08x%n", rs, re);
                rs = a;
            }
            re = a + ins.getLength();
        }
        if (rs >= 0) c.printf("%08x\t%08x%n", rs, re);
        c.close();
        PrintWriter r = new PrintWriter(new File(dir, "ghidra_refs.tsv"));
        ReferenceManager rm = currentProgram.getReferenceManager();
        AddressIterator ai = rm.getReferenceSourceIterator(currentProgram.getMinAddress(), true);
        while (ai.hasNext()) {
            Address from = ai.next();
            for (Reference ref : rm.getReferencesFrom(from)) {
                if (!ref.getToAddress().isMemoryAddress()) continue;
                r.printf("%08x\t%08x\t%s\t%d%n", from.getOffset(), ref.getToAddress().getOffset(), ref.getReferenceType().getName(), ref.getOperandIndex());
            }
        }
        r.close();
        PrintWriter d = new PrintWriter(new File(dir, "ghidra_data.tsv"));
        DataIterator di = listing.getDefinedData(true);
        while (di.hasNext()) {
            Data dt = di.next();
            String v = "";
            if (dt.hasStringValue()) {
                Object o = dt.getValue();
                v = o == null ? "" : o.toString().replace("\t", "\t").replace("\n", "\n").replace("\r", "\r");
            }
            d.printf("%08x\t%d\t%s\t%s%n", dt.getAddress().getOffset(), dt.getLength(), dt.getDataType().getName(), v);
        }
        d.close();
    }
}
