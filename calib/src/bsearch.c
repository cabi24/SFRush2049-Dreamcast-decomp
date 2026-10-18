/* bsearch.c: DC library algorithm (one-based unsigned bounds) as traced in
   pilot/lanes/integer/notes.md. Not the MAT3 body. */
void *bsearch(void *key, void *base, unsigned long nmemb, unsigned long size, int (*compar)(void *, void *))
{
    unsigned long lower;
    unsigned long upper;
    unsigned long mid;
    void *current;
    int result;

    lower = 1;
    upper = nmemb;
    while (lower >= 1 && lower <= upper) {
        mid = lower + ((upper - lower) >> 1);
        current = (char *)base + (mid - 1) * size;
        result = compar(key, current);
        if (result < 0)
            upper = mid - 1;
        if (result > 0)
            lower = mid + 1;
        if (result == 0)
            return current;
    }
    return 0;
}
