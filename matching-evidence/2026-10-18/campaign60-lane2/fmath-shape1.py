from pathlib import Path
p=Path('src/fmath.c');s=p.read_text().replace('    float x,y,z;\n    x=vec[0]; y=vec[1]; z=vec[2];\n    return func_8C026278(x*x+y*y+z*z);','    return func_8C026278(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);');p.write_text(s)
