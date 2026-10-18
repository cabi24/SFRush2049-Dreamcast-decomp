/* Training scaffold only: not equivalent-C, target-compiled, or byte-matched.
 * Derived from the pinned static lane; consult its mapping/ABI limits.
 */
#include <stdint.h>
#include <stddef.h>

/* Defined signed32 arithmetic only. Original int/short return declaration is unresolved.
 * Low32 wrap, division by zero and INT_MIN/-1 need separate target semantics. */
int32_t dc_interp_raw32_candidate(int32_t a, int32_t b, int32_t rem, int32_t total)
{
    return a + rem * (b - a) / total;
}
