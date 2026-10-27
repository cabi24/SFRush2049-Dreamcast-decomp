extern int context_callee(int x);
int context_caller(int x)
{
    int y = context_callee(x);
    return y + x + 5;
}
