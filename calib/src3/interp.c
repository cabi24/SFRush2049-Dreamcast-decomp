/* drivetra.c: interp. MAT3 declares a signed short return; the DC target has no EXTS.W,
   so int and short returns are both candidates (SHC default is NORTNEXT: no extension). */
signed short interp(int a, int b, int rem, int total)
{
    return a + rem * (b - a) / total;
}
