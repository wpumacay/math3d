#pragma once

#include <cmath>
#include <tinymath/vec3_t.hpp>

namespace tiny {
namespace math {
namespace scalar {

// ***************************************************************************//
//   Implementations for single-precision floating point numbers (float32_t)  //
// ***************************************************************************//
using Vec3f = Vector3<float32_t>;
using Array3f = Vec3f::BufferType;

TM_INLINE auto kernel_add_v3f(Array3f& dst, const Array3f& lhs,
                              const Array3f& rhs) -> void {
    for (int32_t i = 0; i < Vec3f::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] + rhs[i];
    }
}

TM_INLINE auto kernel_sub_v3f(Array3f& dst, const Array3f& lhs,
                              const Array3f& rhs) -> void {
    for (int32_t i = 0; i < Vec3f::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] - rhs[i];
    }
}

TM_INLINE auto kernel_scale_v3f(Array3f& dst, float32_t scale,
                                const Array3f& vec) -> void {
    for (int32_t i = 0; i < Vec3f::VECTOR_NDIM; ++i) {
        dst[i] = scale * vec[i];
    }
}

TM_INLINE auto kernel_hadamard_v3f(Array3f& dst, const Array3f& lhs,
                                   const Array3f& rhs) -> void {
    for (int32_t i = 0; i < Vec3f::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] * rhs[i];
    }
}

TM_INLINE auto kernel_length_square_v3f(const Array3f& vec) -> float32_t {
    return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

TM_INLINE auto kernel_normalize_in_place_v3f(Array3f& vec) -> void {
    auto norm = std::sqrt(kernel_length_square_v3f(vec));
    vec[0] = vec[0] / norm;
    vec[1] = vec[1] / norm;
    vec[2] = vec[2] / norm;
}

TM_INLINE auto kernel_dot_v3f(const Array3f& lhs, const Array3f& rhs)
    -> float32_t {
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

TM_INLINE auto kernel_cross_v3f(Array3f& dst, const Array3f& lhs,
                                const Array3f& rhs) -> void {
    // v.x =  v1.y  *  v2.z  -  v1.z  *  v2.y
    dst[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    // v.y =  v1.z  *  v2.x  -  v1.x  *  v2.z
    dst[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    // v.z =  v1.x  *  v2.y  -  v1.y  *  v2.x
    dst[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

TM_INLINE auto kernel_compare_eq_v3f(const Array3f& lhs, const Array3f& rhs)
    -> bool {
    // Check all-close within an epsilon (EPS)
    constexpr auto EPSILON = tiny::math::EPS<float32_t>;
    return (std::abs(lhs[0] - rhs[0]) < EPSILON) &&
           (std::abs(lhs[1] - rhs[1]) < EPSILON) &&
           (std::abs(lhs[2] - rhs[2]) < EPSILON);
}

// ***************************************************************************//
//   Implementations for double-precision floating point numbers (float64_t)  //
// ***************************************************************************//
using Vec3d = Vector3<float64_t>;
using Array3d = Vec3d::BufferType;

TM_INLINE auto kernel_add_v3d(Array3d& dst, const Array3d& lhs,
                              const Array3d& rhs) -> void {
    for (int32_t i = 0; i < Vec3d::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] + rhs[i];
    }
}

TM_INLINE auto kernel_sub_v3d(Array3d& dst, const Array3d& lhs,
                              const Array3d& rhs) -> void {
    for (int32_t i = 0; i < Vec3d::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] - rhs[i];
    }
}

TM_INLINE auto kernel_scale_v3d(Array3d& dst, float64_t scale,
                                const Array3d& vec) -> void {
    for (int32_t i = 0; i < Vec3d::VECTOR_NDIM; ++i) {
        dst[i] = scale * vec[i];
    }
}

TM_INLINE auto kernel_hadamard_v3d(Array3d& dst, const Array3d& lhs,
                                   const Array3d& rhs) -> void {
    for (int32_t i = 0; i < Vec3d::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] * rhs[i];
    }
}

TM_INLINE auto kernel_length_square_v3d(const Array3d& vec) -> float64_t {
    return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

TM_INLINE auto kernel_normalize_in_place_v3d(Array3d& vec) -> void {
    auto norm = std::sqrt(kernel_length_square_v3d(vec));
    vec[0] = vec[0] / norm;
    vec[1] = vec[1] / norm;
    vec[2] = vec[2] / norm;
}

TM_INLINE auto kernel_dot_v3d(const Array3d& lhs, const Array3d& rhs)
    -> float64_t {
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

TM_INLINE auto kernel_cross_v3d(Array3d& dst, const Array3d& lhs,
                                const Array3d& rhs) -> void {
    // v.x =  v1.y  *  v2.z  -  v1.z  *  v2.y
    dst[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    // v.y =  v1.z  *  v2.x  -  v1.x  *  v2.z
    dst[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    // v.z =  v1.x  *  v2.y  -  v1.y  *  v2.x
    dst[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}

TM_INLINE auto kernel_compare_eq_v3d(const Array3d& lhs, const Array3d& rhs)
    -> bool {
    // Check all-close within an epsilon (EPS)
    constexpr auto EPSILON = tiny::math::EPS<float64_t>;
    return (std::abs(lhs[0] - rhs[0]) < EPSILON) &&
           (std::abs(lhs[1] - rhs[1]) < EPSILON) &&
           (std::abs(lhs[2] - rhs[2]) < EPSILON);
}

}  // namespace scalar
}  // namespace math
}  // namespace tiny
