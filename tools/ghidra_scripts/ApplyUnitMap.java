// Headless Ghidra script: apply the unit map (map/functions.csv + map/raw/units.tsv) to the program.
// Creates missing functions at mapped entry points, renames every mapped function (USER_DEFINED),
// adds a plate comment with unit/source/confidence/MAT3 name, and labels + plate-comments each unit start.
// Args: functions.csv path, units.tsv path. Run without -readOnly so the changes are saved.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.util.exception.DuplicateNameException;
import java.io.*;
import java.util.*;

public class ApplyUnitMap extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        Listing listing = currentProgram.getListing();
        SymbolTable st = currentProgram.getSymbolTable();
        int created = 0, renamed = 0, dup = 0, failed = 0;
        // units
        BufferedReader ur = new BufferedReader(new FileReader(args[1]));
        String line;
        while ((line = ur.readLine()) != null) {
            String[] p = line.split("\t");
            if (p.length < 6) continue;
            Address a = toAddr(Long.parseLong(p[0], 16));
            String c = "UNIT " + p[2] + "  [" + p[4] + ", " + p[5] + "]  " + p[0] + ".." + p[1]
                     + ("-".equals(p[3]) ? "" : "  source " + p[3]);
            CodeUnit cu = listing.getCodeUnitAt(a);
            if (cu == null) cu = listing.getCodeUnitContaining(a);
            if (cu != null) {
                String old = cu.getComment(CodeUnit.PLATE_COMMENT);
                if (old == null || !old.startsWith("UNIT ")) cu.setComment(CodeUnit.PLATE_COMMENT, c);
            }
            try {
                st.createLabel(a, "unit_" + p[2], SourceType.USER_DEFINED);
            } catch (Exception e) { }
        }
        ur.close();
        // functions
        BufferedReader fr = new BufferedReader(new FileReader(args[0]));
        fr.readLine();
        // first pass: collect mapped entries; remove stale USER_DEFINED functions from earlier runs of this script
        Set<Long> mapped = new HashSet<>();
        List<String> lines = new ArrayList<>();
        while ((line = fr.readLine()) != null) {
            lines.add(line);
            String[] p = line.split(",", 8);
            if (p.length >= 3) mapped.add(Long.parseLong(p[2].substring(2), 16));
        }
        fr.close();
        int removed = 0, foreign = 0;
        List<Function> stale = new ArrayList<>();
        for (Function f : listing.getFunctions(true)) {
            if (mapped.contains(f.getEntryPoint().getOffset())) continue;
            if (f.getSymbol().getSource() == SourceType.USER_DEFINED) stale.add(f); else foreign++;
        }
        for (Function f : stale) { listing.removeFunction(f.getEntryPoint()); removed++; }
        println("ApplyUnitMap: removed " + removed + " stale user-defined functions; " + foreign + " analyzer functions not in the map left as is");
        for (String ln : lines) {
            String[] p = ln.split(",", 8);
            if (p.length < 7) continue;
            String unit = p[0], name = p[1], mat3 = p[5], conf = p[6];
            Address a = toAddr(Long.parseLong(p[2].substring(2), 16));
            Function fn = getFunctionAt(a);
            if (fn == null) {
                Function cont = getFunctionContaining(a);
                if (cont != null && !cont.getEntryPoint().equals(a)) {
                    // our map splits an existing Ghidra body; shrink it by re-creating at our entry
                }
                try {
                    if (listing.getInstructionAt(a) == null) {
                        Address e = toAddr(Long.parseLong(p[3].substring(2), 16) - 1);
                        // analyzer left data here: clear defined data (not instructions) in the mapped extent first
                        DataIterator di = listing.getDefinedData(new ghidra.program.model.address.AddressSet(a, e), true);
                        while (di.hasNext()) { Data d = di.next(); clearListing(d.getMinAddress(), d.getMaxAddress()); }
                        disassemble(a);
                    }
                    fn = createFunction(a, name);
                    if (fn != null) created++;
                } catch (Exception e) { fn = null; }
            }
            if (fn == null) { failed++; continue; }
            try {
                fn.setName(name, SourceType.USER_DEFINED);
                renamed++;
            } catch (DuplicateNameException e) {
                try { fn.setName(name + "__" + unit, SourceType.USER_DEFINED); renamed++; dup++; }
                catch (Exception e2) { failed++; }
            }
            String pc = "unit " + unit + " | confidence " + conf + (mat3.isEmpty() ? "" : " | MAT3 " + mat3);
            fn.setComment(pc);
        }
        println("ApplyUnitMap: created " + created + ", renamed " + renamed + " (" + dup + " with unit suffix), failed " + failed);
    }
}
