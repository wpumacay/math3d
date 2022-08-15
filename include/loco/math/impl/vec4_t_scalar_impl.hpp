#pragma once

#include <loco/math/vec4_t.hpp>

namespace loco {
namespace math {
namespace scalar {

template <typename T>
using Vec4Buffer = typename Vector4<T>::BufferType;

template <typename T>
using SFINAE_VEC4_SCALAR_GUARD =
    typename std::enable_if<IsScalar<T>::value>::type*;

template <typename T, SFINAE_VEC4_SCALAR_GUARD<T> = nullptr>
LM_INLINE auto kernel_add_vec4(Vec4Buffer<T>& dst, const Vec4Buffer<T>& lhs,
                               const Vec4Buffer<T>& rhs) -> void {
    for (uint32_t i = 0; i < Vector4<T>::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] + rhs[i];
    }
}

template <typename T, SFINAE_VEC4_SCALAR_GUARD<T> = nullptr>
LM_INLINE auto kernel_sub_vec4(Vec4Buffer<T>& dst, const Vec4Buffer<T>& lhs,
                               const Vec4Buffer<T>& rhs) -> void {
    for (uint32_t i = 0; i < Vector4<T>::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] - rhs[i];
    }
}

template <typename T, SFINAE_VEC4_SCALAR_GUARD<T> = nullptr>
LM_INLINE auto kernel_scale_vec4(Vec4Buffer<T>& dst, T scale,
                                 const Vec4Buffer<T>& vec) -> void {
    for (uint32_t i = 0; i < Vector4<T>::VECTOR_NDIM; ++i) {
        dst[i] = scale * vec[i];
    }
}

template <typename T, SFINAE_VEC4_SCALAR_GUARD<T> = nullptr>
LM_INLINE auto kernel_hadamard_vec4(Vec4Buffer<T>& dst,
                                    const Vec4Buffer<T>& lhs,
                                    const Vec4Buffer<T>& rhs) -> void {
    for (uint32_t i = 0; i < Vector4<T>::VECTOR_NDIM; ++i) {
        dst[i] = lhs[i] * rhs[i];
    }
}

template <typename T, SFINAE_VEC4_SCALAR_GUARD<T> = nullptr>
LM_INLINE auto kernel_dot_vec4(const Vec4Buffer<T>& lhs,
                               const Vec4Buffer<T>& rhs) -> T {
    T accum = static_cast<T>(0.0);
    for (uint32_t i = 0; i < Vector4<T>::VECTOR_NDIM; ++i) {
        accum += lhs[i] * rhs[i];
    }
    return accum;
}

template <typename T, SFINAE_VEC4_SCALAR_GUARD<T> = nullptr>
LM_INLINE auto kernel_compare_eq_vec4(const Vec4Buffer<T>& lhs,
                                      const Vec4Buffer<T>& rhs) -> bool {
    for (uint32_t i = 0; i < Vector4<T>::VECTOR_NDIM; ++i) {
        if (std::abs(lhs[i] - rhs[i]) >= static_cast<T>(loco::math::EPS)) {
            return false;
        }
    }
    return true;
}

}  // namespace scalar
}  // namespace math
}  // namespace loco