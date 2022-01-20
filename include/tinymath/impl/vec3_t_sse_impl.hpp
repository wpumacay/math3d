#pragma once

#if defined(TINYMATH_SSE_ENABLED)

#include <emmintrin.h>
#include <smmintrin.h>
#include <xmmintrin.h>

#include <tinymath/vec3_t.hpp>
#include <type_traits>

/**
 * SSE instruction sets required for each kernel:
 *
 * - kernel_add_vec3                : SSE|SSE2
 * - kernel_sub_vec3                : SSE|SSE2
 * - kernel_scale_vec3              : SSE|SSE2
 * - kernel_hadamard_vec3           : SSE|SSE2
 * - kernel_length_square_vec3      : SSE|SSE2|SSE4.1 (_mm_dp_ps)
 * - kernel_length_vec3             : SSE|SSE2|SSE4.1 (_mm_dp_ps)
 * - kernel_normalize_in_place_vec3 : SSE|SSE2|SSE4.1 (_mm_dp_ps)
 * - kernel_dot_vec3                : SSE|SSE2|SSE4.1 (_mm_dp_ps)
 * - kernel_cross_vec3              : SSE
 */

namespace tiny {
namespace math {
namespace sse {

template <typename T>
using ArrayBuffer = typename Vector3<T>::BufferType;

template <typename T>
constexpr int32_t VECTOR_NDIM = Vector3<T>::VECTOR_NDIM;

template <typename T>
constexpr int32_t BUFFER_SIZE = Vector3<T>::BUFFER_SIZE;

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_add_vec3(ArrayBuffer<T>& dst, const ArrayBuffer<T>& lhs,
                               const ArrayBuffer<T>& rhs) -> void {
    // All elements of the buffer (4xf32, recall padding for alignment) fit into
    // a single xmm register (128-bits <=> 4xfloat32)
    auto xmm_lhs = _mm_load_ps(lhs.data());
    auto xmm_rhs = _mm_load_ps(rhs.data());
    auto xmm_result = _mm_add_ps(xmm_lhs, xmm_rhs);
    _mm_store_ps(dst.data(), xmm_result);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_add_vec3(ArrayBuffer<T>& dst, const ArrayBuffer<T>& lhs,
                               const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    // Vector buffer contains 4xfloat64 <=> 256 bits <=> 32 bytes; however, xmm
    // registers are only 16 bytes wide. We'll just unroll the loop once (make
    // the point-wise operation twice on the registers, using lo-hi parts). Note
    // that we also require support for SSE2 to use the appropriate intrinsics
    // @todo(wilbert): try using static_cast and pointer-arithmetic replacements
    auto xmm_lhs_lo = _mm_load_pd(lhs.data());
    auto xmm_lhs_hi = _mm_load_pd(lhs.data() + 2);
    auto xmm_rhs_lo = _mm_load_pd(rhs.data());
    auto xmm_rhs_hi = _mm_load_pd(rhs.data() + 2);
    auto xmm_result_lo = _mm_add_pd(xmm_lhs_lo, xmm_rhs_lo);
    auto xmm_result_hi = _mm_add_pd(xmm_lhs_hi, xmm_rhs_hi);
    _mm_store_pd(dst.data(), xmm_result_lo);
    _mm_store_pd(dst.data() + 2, xmm_result_hi);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_sub_vec3(ArrayBuffer<T>& dst, const ArrayBuffer<T>& lhs,
                               const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    auto xmm_lhs = _mm_load_ps(lhs.data());
    auto xmm_rhs = _mm_load_ps(rhs.data());
    auto xmm_result = _mm_sub_ps(xmm_lhs, xmm_rhs);
    _mm_store_ps(dst.data(), xmm_result);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_sub_vec3(ArrayBuffer<T>& dst, const ArrayBuffer<T>& lhs,
                               const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    auto xmm_lhs_lo = _mm_load_pd(lhs.data());
    auto xmm_lhs_hi = _mm_load_pd(lhs.data() + 2);
    auto xmm_rhs_lo = _mm_load_pd(rhs.data());
    auto xmm_rhs_hi = _mm_load_pd(rhs.data() + 2);
    auto xmm_result_lo = _mm_sub_pd(xmm_lhs_lo, xmm_rhs_lo);
    auto xmm_result_hi = _mm_sub_pd(xmm_lhs_hi, xmm_rhs_hi);
    _mm_store_pd(dst.data(), xmm_result_lo);
    _mm_store_pd(dst.data() + 2, xmm_result_hi);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_scale_vec3(ArrayBuffer<T>& dst, T scale,
                                 const ArrayBuffer<T>& vec) -> void {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    auto xmm_scale = _mm_set1_ps(scale);
    auto xmm_vector = _mm_load_ps(vec.data());
    auto xmm_result = _mm_mul_ps(xmm_scale, xmm_vector);
    _mm_store_ps(dst.data(), xmm_result);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_scale_vec3(ArrayBuffer<T>& dst, T scale,
                                 const ArrayBuffer<T>& vec) -> void {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    auto xmm_scale = _mm_set1_pd(scale);
    auto xmm_vector_lo = _mm_load_pd(vec.data());
    auto xmm_vector_hi = _mm_load_pd(vec.data() + 2);
    auto xmm_result_lo = _mm_mul_pd(xmm_scale, xmm_vector_lo);
    auto xmm_result_hi = _mm_mul_pd(xmm_scale, xmm_vector_hi);
    _mm_store_pd(dst.data(), xmm_result_lo);
    _mm_store_pd(dst.data() + 2, xmm_result_hi);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_hadamard_vec3(ArrayBuffer<T>& dst,
                                    const ArrayBuffer<T>& lhs,
                                    const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    auto xmm_lhs = _mm_load_ps(lhs.data());
    auto xmm_rhs = _mm_load_ps(rhs.data());
    _mm_store_ps(dst.data(), _mm_mul_ps(xmm_lhs, xmm_rhs));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_hadamard_vec3(ArrayBuffer<T>& dst,
                                    const ArrayBuffer<T>& lhs,
                                    const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    auto xmm_lhs_lo = _mm_load_pd(lhs.data());
    auto xmm_lhs_hi = _mm_load_pd(lhs.data() + 2);
    auto xmm_rhs_lo = _mm_load_pd(rhs.data());
    auto xmm_rhs_hi = _mm_load_pd(rhs.data() + 2);
    _mm_store_pd(dst.data(), _mm_mul_pd(xmm_lhs_lo, xmm_rhs_lo));
    _mm_store_pd(dst.data() + 2, _mm_mul_pd(xmm_lhs_hi, xmm_rhs_hi));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_length_square_vec3(const ArrayBuffer<T>& vec) -> T {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    // Implementation based on this post: https://bit.ly/3FyZF0n
    constexpr int32_t COND_PROD_MASK = 0x71;
    auto xmm_v = _mm_load_ps(vec.data());
    return _mm_cvtss_f32(_mm_dp_ps(xmm_v, xmm_v, COND_PROD_MASK));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_length_square_vec3(const ArrayBuffer<T>& vec) -> T {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    // Implementation based on this post: https://bit.ly/3FyZF0n
    constexpr int32_t COND_PROD_MASK = 0x31;
    auto xmm_v_lo = _mm_load_pd(vec.data());
    auto xmm_v_hi = _mm_load_pd(vec.data() + 2);
    auto xmm_square_sum_lo = _mm_dp_pd(xmm_v_lo, xmm_v_lo, COND_PROD_MASK);
    auto xmm_square_sum_hi = _mm_dp_pd(xmm_v_hi, xmm_v_hi, COND_PROD_MASK);
    auto xmm_square_sum = _mm_add_pd(xmm_square_sum_lo, xmm_square_sum_hi);
    return _mm_cvtsd_f64(xmm_square_sum);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_length_vec3(const ArrayBuffer<T>& vec) -> T {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    // Implementation based on this post: https://bit.ly/3FyZF0n
    constexpr int32_t COND_PROD_MASK = 0x71;
    auto xmm_v = _mm_load_ps(vec.data());
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(xmm_v, xmm_v, COND_PROD_MASK)));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_length_vec3(const ArrayBuffer<T>& vec) -> T {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    // Implementation based on this post: https://bit.ly/3FyZF0n
    constexpr int32_t COND_PROD_MASK = 0x31;
    auto xmm_v_01 = _mm_load_pd(vec.data());
    auto xmm_v_23 = _mm_load_pd(vec.data() + 2);
    auto xmm_square_sum_01 = _mm_dp_pd(xmm_v_01, xmm_v_01, COND_PROD_MASK);
    auto xmm_square_sum_23 = _mm_dp_pd(xmm_v_23, xmm_v_23, COND_PROD_MASK);
    auto xmm_square_sum = _mm_add_pd(xmm_square_sum_01, xmm_square_sum_23);
    return _mm_cvtsd_f64(_mm_sqrt_sd(xmm_square_sum, xmm_square_sum));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_normalize_in_place_vec3(ArrayBuffer<T>& vec) -> void {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    // Implementation based on this post: https://bit.ly/3FyZF0n
    constexpr int32_t COND_PROD_MASK = 0x7f;
    auto xmm_v = _mm_load_ps(vec.data());
    auto xmm_sums = _mm_dp_ps(xmm_v, xmm_v, COND_PROD_MASK);
    auto xmm_r_sqrt_sums = _mm_sqrt_ps(xmm_sums);
    auto xmm_v_norm = _mm_div_ps(xmm_v, xmm_r_sqrt_sums);
    _mm_store_ps(vec.data(), xmm_v_norm);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_normalize_in_place_vec3(ArrayBuffer<T>& vec) -> void {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    // Implementation based on this post: https://bit.ly/3FyZF0n
    constexpr int32_t COND_PROD_MASK = 0x33;
    auto xmm_v_01 = _mm_load_pd(vec.data());
    auto xmm_v_23 = _mm_load_pd(vec.data() + 2);
    auto xmm_sums_01 = _mm_dp_pd(xmm_v_01, xmm_v_01, COND_PROD_MASK);
    auto xmm_sums_23 = _mm_dp_pd(xmm_v_23, xmm_v_23, COND_PROD_MASK);
    auto xmm_r_sqrt_sums = _mm_sqrt_pd(_mm_add_pd(xmm_sums_01, xmm_sums_23));
    auto xmm_v_norm_01 = _mm_div_pd(xmm_v_01, xmm_r_sqrt_sums);
    auto xmm_v_norm_23 = _mm_div_pd(xmm_v_23, xmm_r_sqrt_sums);
    _mm_store_pd(vec.data(), xmm_v_norm_01);
    _mm_store_pd(vec.data() + 2, xmm_v_norm_23);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_dot_vec3(const ArrayBuffer<T>& lhs,
                               const ArrayBuffer<T>& rhs) -> T {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    constexpr int32_t COND_PROD_MASK = 0x71;
    auto xmm_lhs = _mm_load_ps(lhs.data());
    auto xmm_rhs = _mm_load_ps(rhs.data());
    auto xmm_cond_prod = _mm_dp_ps(xmm_lhs, xmm_rhs, COND_PROD_MASK);
    return _mm_cvtss_f32(xmm_cond_prod);
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_dot_vec3(const ArrayBuffer<T>& lhs,
                               const ArrayBuffer<T>& rhs) -> T {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    constexpr int32_t COND_PROD_MASK = 0x31;
    auto xmm_lhs_01 = _mm_load_pd(lhs.data());
    auto xmm_lhs_23 = _mm_load_pd(lhs.data() + 2);
    auto xmm_rhs_01 = _mm_load_pd(rhs.data());
    auto xmm_rhs_23 = _mm_load_pd(rhs.data() + 2);
    auto xmm_dot_01 = _mm_dp_pd(xmm_lhs_01, xmm_rhs_01, COND_PROD_MASK);
    auto xmm_dot_23 = _mm_dp_pd(xmm_lhs_23, xmm_rhs_23, COND_PROD_MASK);
    return _mm_cvtsd_f64(_mm_add_pd(xmm_dot_01, xmm_dot_23));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat32<T>::value>::type* = nullptr>
TM_INLINE auto kernel_cross_vec3(ArrayBuffer<T>& dst, const ArrayBuffer<T>& lhs,
                                 const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<float, T>::value, "We must be using f32");
    // Implementation adapted from @ian_mallett (https://bit.ly/3lu6pVe)
    constexpr auto MASK_A = tiny::math::ShuffleMask<3, 0, 2, 1>::value;
    constexpr auto MASK_B = tiny::math::ShuffleMask<3, 1, 0, 2>::value;
    // Recall that the dot product of two 3d-vectors a and b given by:
    // a = {a[0], a[1], a[2], a[3]=0}, b = {b[0], b[1], b[2], b[3]=0}
    // has as resulting expression:
    // a (x) b = [a[1] * b[2] - a[2] * b[1],
    //            a[2] * b[0] - a[0] * b[2],
    //            a[0] * b[1] - a[1] * b[0],
    //                        0            ]
    auto vec_a = _mm_load_ps(lhs.data());  // a = {a[0], a[1], a[2], a[3]=0}
    auto vec_b = _mm_load_ps(rhs.data());  // b = {b[0], b[1], b[2], b[3]=0}
    // tmp_0 = {a[1], a[2], a[0], 0}
    auto tmp_0 = _mm_shuffle_ps(vec_a, vec_a, MASK_A);
    // tmp_1 = {b[2], b[0], b[1], 0}
    auto tmp_1 = _mm_shuffle_ps(vec_b, vec_b, MASK_B);
    // tmp_2 = {a[2], a[0], a[1], 0}
    auto tmp_2 = _mm_shuffle_ps(vec_a, vec_a, MASK_B);
    // tmp_3 = {b[1], b[2], b[0], 0}
    auto tmp_3 = _mm_shuffle_ps(vec_b, vec_b, MASK_A);
    _mm_store_ps(dst.data(), _mm_sub_ps(_mm_mul_ps(tmp_0, tmp_1),
                                        _mm_mul_ps(tmp_2, tmp_3)));
}

template <typename T,
          typename std::enable_if<CpuHasSSE<T>::value &&
                                  IsFloat64<T>::value>::type* = nullptr>
TM_INLINE auto kernel_cross_vec3(ArrayBuffer<T>& dst, const ArrayBuffer<T>& lhs,
                                 const ArrayBuffer<T>& rhs) -> void {
    static_assert(std::is_same<double, T>::value, "We must be using f64");
    // @todo(wilbert): so far I can't find a way that might be better than
    // the scalar implementation (besides it might be vectorized by O3)
    dst[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    dst[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    dst[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

}  // namespace sse
}  // namespace math
}  // namespace tiny

#endif  // TINYMATH_SSE_ENABLED
