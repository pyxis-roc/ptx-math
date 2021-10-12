#include "models.h"

#include "common/bitcast.h"
#include "common/nan.h"
#include "common/util.h"

#include <math.h>

uint32_t SASS_MUFU_RCP64H(uint32_t x)
{
    const double x_full = u64_as_double((uint64_t)x << 32);

    switch (fpclassify(x_full))
    {
    case FP_NAN:
        return PTX_CANONICAL_NAN;
    case FP_INFINITE:
        return x & 0x80000000u;                     // signed zero
    case FP_ZERO:
    case FP_SUBNORMAL:
        return (x & 0x80000000u) | (0x7ffu << 20);  // signed infinity
    }

    const uint32_t sign = x >> 31;
    const uint32_t exp = (x >> 20) & MASK_U32(11);
    const uint32_t frac = x & MASK_U32(20);

    const int x_log2 = (int)exp - 1023;

    if (x_log2 >= 1022 && (x_log2 > 1022 || frac != 0u))
        return x & 0x80000000u;

    const uint32_t in_bits = FP_FORMAT(sign, 127u, frac << 3);

    const float in = u32_as_float(in_bits);
    const float rcp = ptxm_rcp_sm5x(in);

    const double r = ldexp(rcp, -x_log2);

    return double_as_u64(r) >> 32;
}
