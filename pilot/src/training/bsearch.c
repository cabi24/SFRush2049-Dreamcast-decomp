/* Training scaffold only: not equivalent-C, target-compiled, or byte-matched.
 * Derived from the pinned static lane; consult its mapping/ABI limits.
 */
#include <stdint.h>
#include <stddef.h>

/* Distinct DC algorithm; importing the MAT3 body would change zero-size behavior. */
_Static_assert(sizeof(void *) == 4, "32-bit target pointers required");
void *dc_bsearch_candidate(void *key, void *base, uint32_t nmemb,
                           uint32_t size, int (*compar)(void *, void *))
{
    uint32_t lower = 1;
    uint32_t upper = nmemb;
    while (lower >= 1 && lower <= upper) {
        uint32_t mid = lower + ((upper - lower) >> 1);
        unsigned char *current = (unsigned char *)base + (mid - 1) * size;
        int result = compar(key, current);
        if (result < 0) upper = mid - 1;
        if (result > 0) lower = mid + 1;
        if (result == 0) return current;
    }
    return NULL;
}
