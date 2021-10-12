#include "models.h"

#include "common/bitcast.h"
#include "common/nan.h"
#include "common/util.h"

#include <math.h>

uint32_t SASS_MUFU_RSQ64H(uint32_t x)
{
    const double x_full = u64_as_double((uint64_t)x << 32);

    switch (fpclassify(x_full))
    {
    case FP_NAN:
        return PTX_CANONICAL_NAN;
    case FP_INFINITE:
        return signbit(x_full) ? PTX_CANONICAL_NAN : 0u;
    case FP_ZERO:
    case FP_SUBNORMAL:
        return (x & 0x80000000u) | (0x7ffu << 20);  // signed infinity
    }

    const uint32_t sign = x >> 31;
    const uint32_t exp = (x >> 20) & MASK_U32(11);
    const uint32_t frac = x & MASK_U32(20);

    int x_log2 = (int)exp - 1023;

    uint32_t in_exp = 127u;

    if (x_log2 % 2 != 0)
    {
        x_log2 -= 1;
        in_exp += 1u;
    }

    const uint32_t in_bits = FP_FORMAT(sign, in_exp, frac << 3);

    const float in = u32_as_float(in_bits);
    const float rsqrt = ptxm_rsqrt_sm5x(in);

    const double r = ldexp(rsqrt, -x_log2 / 2);

    return double_as_u64(r) >> 32;
}
