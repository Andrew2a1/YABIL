#pragma once

#include <yabil/bigint/algorithms_config.h>
#include <yabil/bigint/bigint_export.h>

#include <cstdint>

#if YABIL_CONFIG_CONSTEVAL_THRESHOLDS
#define YABIL_CONSTEXPR_PREFIX static inline constexpr
#endif

namespace yabil::bigint
{

/// @brief Structure containing all Thresholds for bigint algorithms
struct YABIL_BIGINT_EXPORT Thresholds
{
    YABIL_CONSTEXPR_PREFIX uint64_t karatsuba_threshold_digits = YABIL_CONFIG_KARATSUBA_THRESHOLD;
    YABIL_CONSTEXPR_PREFIX uint64_t recursive_div_threshold_digits = YABIL_CONFIG_RECURSIVE_DIV_THRESHOLD;
    YABIL_CONSTEXPR_PREFIX uint64_t parallel_add_digits = YABIL_CONFIG_PARALLEL_ADD_THRESHOLD;
    YABIL_CONSTEXPR_PREFIX uint64_t parallel_mul_digits = YABIL_CONFIG_PARALLEL_MUL_THRESHOLD;
    YABIL_CONSTEXPR_PREFIX uint64_t parallel_div_digits = YABIL_CONFIG_PARALLEL_DIV_THRESHOLD;
};

}  // namespace yabil::bigint
