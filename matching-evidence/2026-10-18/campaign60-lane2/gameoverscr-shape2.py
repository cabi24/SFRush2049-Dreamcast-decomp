from pathlib import Path
p=Path('src/gameoverscr.c');s=p.read_text().replace('void setGameOverState(int newState)\n{','void setGameOverState(volatile int newState)\n{');p.write_text(s)
