from pathlib import Path
p=Path('src/gameoverscr.c');s=p.read_text().replace('void setGameOverState(int newState);','void setGameOverState(volatile int newState);');p.write_text(s)
