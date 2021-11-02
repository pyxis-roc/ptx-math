#include "models.h"
#include "tuning.h"

#include "tables/ex2_table.h"
#include "reduction/rro_ex2.h"
#include "common/bitcast.h"
#include "common/nan.h"
#include "common/squarer.h"
#include "common/util.h"

#include <stdbool.h>
#include <math.h>

static float mufu_ex2(uint32_t reduced, const ptxm_params *params);

static int min(int a, int b);
static float fexp2i(int n);

static const ptxm_params model_params =
{
    .table = ptxm_ex2_table,
    .bias = UINT64_C(0x6fc4000000000000)
};

float ptxm_ex2_sm5x(float x)
{
    if (isnan(x)) return ptxm_nan();

    bool square_result = false;

    if (x < -126.0f)
    {
        x *= 0.5f;
        square_result = true;
    }

    const float r = mufu_ex2(ptxm_rro_ex2_sm5x(x), &model_params);

    return square_result ? r * r : r;
}

float SASS_RRO_EX2(float x)
{
    return u32_as_float(ptxm_rro_ex2_sm5x(x));
}

float SASS_MUFU_EX2(float x)
{
    return mufu_ex2(float_as_u32(x), &model_params);
}

float mufu_ex2(uint32_t reduced, const ptxm_params *params)
{
    uint32_t sign = reduced >> 31;
    int32_t integral = (reduced >> 23) & MASK_U32(8);

    switch (integral & 0x81u)
    {
    case 0x80u:
        return ptxm_nan();
    case 0x81u:
        return sign ? 0.0f : INFINITY;
    }

    if (sign)
    {
        integral = -integral;

        if (reduced & MASK_U32(23))
        {
            reduced = ~reduced;
            integral -= 1;
        }
    }

    const uint32_t xh = UPPER_SIGNIFICAND(reduced, EX2_M);
    const uint32_t xl = LOWER_SIGNIFICAND(reduced, EX2_M);

    const uint32_t *const c = params->table[xh];

    uint64_t c0_term = c[0];
    uint64_t c1_term = c[1] * (uint64_t)xl;
    uint64_t c2_term = c[2] * ptxm_square_approx(xl);

    c0_term <<= EX2_C0_TERM_ALIGNMENT;
    c1_term <<= EX2_C1_TERM_ALIGNMENT;
    c2_term <<= EX2_C2_TERM_ALIGNMENT;

    uint64_t sum = c0_term + c1_term + c2_term;

    sum += params->bias >> ((64 - EX2_SUM_WEIGHT) + 23);

    const uint32_t r_frac = EXTRACT_BITS(sum, 23, EX2_SUM_WEIGHT);
    const uint32_t r_bits = FP_FORMAT(0u, 127u, r_frac);

    const float r = u32_as_float(r_bits);

    return fexp2i(integral) * r;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

float fexp2i(int n)
{
    if (n > 127) return INFINITY;
    if (n < -126) return 0.0f;

    return ldexpf(1.0f, n);
}
