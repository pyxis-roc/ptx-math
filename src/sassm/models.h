#ifndef PTXM_MODELS_H
#define PTXM_MODELS_H

#ifdef __cplusplus
extern "C" {
#endif

float ptxm_rcp_sm5x(float x);
float ptxm_sqrt_sm5x(float x);
float ptxm_sqrt_sm6x(float x);
float ptxm_rsqrt_sm5x(float x);
float ptxm_sin_sm5x(float x);
float ptxm_sin_sm70(float x);
float ptxm_cos_sm5x(float x);
float ptxm_cos_sm70(float x);
float ptxm_lg2_sm5x(float x);
float ptxm_ex2_sm5x(float x);

float SASS_MUFU_RCP(float x);
float SASS_MUFU_COS(float x);
float SASS_MUFU_SIN(float x);
float SASS_RRO_SINCOS(float x);
float SASS_MUFU_LG2(float x);
float SASS_MUFU_RSQ(float x);
float SASS_MUFU_SQRT(float x);
float SASS_RRO_EX2(float x);
float SASS_MUFU_EX2(float x);

/* TODO

uint32_t SASS_MUFU_RSQ64H(uint32_t x);
uint32_t SASS_MUFU_RCP64H(uint32_t x);

*/
#ifdef __cplusplus
}   // extern "C"
#endif

#define ptxm_rcp_sm6x ptxm_rcp_sm5x
#define ptxm_rcp_sm70 ptxm_rcp_sm5x
#define ptxm_rcp_sm75 ptxm_rcp_sm5x

#define ptxm_sqrt_sm70 ptxm_sqrt_sm6x
#define ptxm_sqrt_sm75 ptxm_sqrt_sm6x

#define ptxm_rsqrt_sm6x ptxm_rsqrt_sm5x
#define ptxm_rsqrt_sm70 ptxm_rsqrt_sm5x
#define ptxm_rsqrt_sm75 ptxm_rsqrt_sm5x

#define ptxm_sin_sm6x ptxm_sin_sm5x
#define ptxm_sin_sm75 ptxm_sin_sm70

#define ptxm_cos_sm6x ptxm_cos_sm5x
#define ptxm_cos_sm75 ptxm_cos_sm70

#define ptxm_lg2_sm6x ptxm_lg2_sm5x
#define ptxm_lg2_sm70 ptxm_lg2_sm5x
#define ptxm_lg2_sm75 ptxm_lg2_sm5x

#define ptxm_ex2_sm6x ptxm_ex2_sm5x
#define ptxm_ex2_sm70 ptxm_ex2_sm5x
#define ptxm_ex2_sm75 ptxm_ex2_sm5x

#endif
