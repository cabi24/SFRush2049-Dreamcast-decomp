from pathlib import Path
p=Path('src/model.c');s=p.read_text().replace('0.03f * target_catchup + 0.97000003f * model[i].catchup','0.97000003f * model[i].catchup + 0.03f * target_catchup');p.write_text(s)
