#include "models.h"
#include "tuning.h"

#include "tables/rcp_table.h"
#include "common/bitcast.h"
#include "common/nan.h"
#include "common/squarer.h"
#include "common/util.h"

#include <math.h>

static const ptxm_params model_params =
{
    .table = ptxm_rcp_table,
    .bias = UINT64_C(0x67e7000000000000)
};

float sass_mufu_rcp_sm6x_internal(float x, const ptxm_params *params)
{
    switch(fpclassify(x))
    {
        case FP_NAN:
            return ptxm_nan();
        case FP_INFINITE:
            return copysignf(0.0f, x);
        case FP_ZERO:
            return copysignf(INFINITY, x);
        case FP_SUBNORMAL:
            return copysignf(INFINITY, x);
    }
    int x_log2 = ilogbf(x);

    const uint32_t x_bits = float_as_u32(x);

    const uint32_t xh = UPPER_SIGNIFICAND(x_bits, RCP_M);
    const uint32_t xl = LOWER_SIGNIFICAND(x_bits, RCP_M);

    const uint32_t *const c = params->table[xh];

    uint64_t c0_term = c[0];
    uint64_t c1_term = c[1] * xl;   // won't exceed 32 bits
    uint64_t c2_term = c[2] * (ptxm_square_approx(xl << 1) >> 2);

    c0_term <<= RCP_C0_TERM_ALIGNMENT;
    c1_term <<= RCP_C1_TERM_ALIGNMENT;
    c2_term <<= RCP_C2_TERM_ALIGNMENT;

    uint64_t sum = c0_term - c1_term + c2_term;

    sum += params->bias >> ((64 - RCP_SUM_WEIGHT) + 23);

    uint32_t r_exp = 127u;

    if (!(sum >> RCP_SUM_WEIGHT))
    {
        r_exp -= 1u;
        sum <<= 1;
    }

    const uint32_t r_frac = EXTRACT_BITS(sum, 23, RCP_SUM_WEIGHT);
    const uint32_t r_sign = signbit(x) ? 1u : 0u;

    const uint32_t r_bits = FP_FORMAT(r_sign, r_exp, r_frac);

    const float r = u32_as_float(r_bits);

    return ldexpf(r, -x_log2);

}

float SASS_MUFU_RCP(float x)
{
    return sass_mufu_rcp_sm6x_internal(x, &model_params);
}

float ptxm_rcp_sm5x(float x)
{
    return ptxm_rcp_sm5x_internal(x, &model_params);
}

float ptxm_rcp_sm5x_internal(float x, const ptxm_params *params)
{
    int x_log2 = 0;

    switch (fpclassify(x))
    {
    case FP_NAN:
        return ptxm_nan();
    case FP_INFINITE:
        return copysignf(0.0f, x);
    case FP_ZERO:
        return copysignf(INFINITY, x);
    case FP_SUBNORMAL:
        x_log2 = -24;
        x *= 0x1p24f;
        break;
    }

    x_log2 += ilogbf(x);

    const uint32_t x_bits = float_as_u32(x);

    const uint32_t xh = UPPER_SIGNIFICAND(x_bits, RCP_M);
    const uint32_t xl = LOWER_SIGNIFICAND(x_bits, RCP_M);

    const uint32_t *const c = params->table[xh];

    uint64_t c0_term = c[0];
    uint64_t c1_term = c[1] * xl;   // won't exceed 32 bits
    uint64_t c2_term = c[2] * (ptxm_square_approx(xl << 1) >> 2);

    c0_term <<= RCP_C0_TERM_ALIGNMENT;
    c1_term <<= RCP_C1_TERM_ALIGNMENT;
    c2_term <<= RCP_C2_TERM_ALIGNMENT;

    uint64_t sum = c0_term - c1_term + c2_term;

    sum += params->bias >> ((64 - RCP_SUM_WEIGHT) + 23);

    uint32_t r_exp = 127u;

    if (!(sum >> RCP_SUM_WEIGHT))
    {
        r_exp -= 1u;
        sum <<= 1;
    }

    const uint32_t r_frac = EXTRACT_BITS(sum, 23, RCP_SUM_WEIGHT);
    const uint32_t r_sign = signbit(x) ? 1u : 0u;

    const uint32_t r_bits = FP_FORMAT(r_sign, r_exp, r_frac);

    const float r = u32_as_float(r_bits);

    return ldexpf(r, -x_log2);
}
