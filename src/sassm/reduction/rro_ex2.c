#include "rro_ex2.h"

#include "../common/bitcast.h"
#include "../common/util.h"

#include <math.h>

static int min(int a, int b);

uint32_t ptxm_rro_ex2_sm5x(float x)
{
    const uint32_t sign = signbit(x) ? 1u : 0u;

    if (isnan(x)) return FP_FORMAT(sign, 0x80u, 0u);

    if (x >= 128.0f) return FP_FORMAT(0u, 0x81u, 0u);
    if (x <= -128.0f) return FP_FORMAT(1u, 0x81u, 0u);

    float integral;
    x = modff(x, &integral);

    uint32_t x_bits = float_as_u32(x);

    if (x != 0.0f)
    {
        x_bits |= UINT32_C(1) << 23;
        x_bits &= MASK_U32(24);

        x_bits >>= min(-ilogbf(x), 24);
    }

    const uint32_t i = fabsf(integral);
    const uint32_t r = x_bits & MASK_U32(23);

    return FP_FORMAT(sign, i, r);
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}
