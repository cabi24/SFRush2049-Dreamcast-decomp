from pathlib import Path
p=Path('src/controlsys.c');s=Path('matching-evidence/2026-10-18/campaign100-lane2/controlsys.original.c').read_text();s=s.replace('((volatile tPadInfo *)gControllerData)[i].buttons &=0xffffcfff;', '((volatile unsigned int *)&gControllerData[0].buttons)[i * (sizeof(tPadInfo)/sizeof(unsigned int))] &=0xffffcfff;');p.write_text(s)
