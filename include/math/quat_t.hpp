#pragma once

#include <cmath>

#include <math/quat_t_decl.hpp>

#include <math/impl/quat_t_scalar_impl.hpp>
#include <math/impl/quat_t_sse_impl.hpp>
#include <math/impl/quat_t_avx_impl.hpp>

#include <math/vec3_t.hpp>
#include <math/euler_t.hpp>

namespace math {

// ***************************************************************************//
//                            Factory functions                               //
// ***************************************************************************//

template <typename T>
auto Quaternion<T>::setFromRotationMatrix(const Mat3& m) -> void {
    // clang-format off
    auto m00 = m(0, 0); auto m01 = m(0, 1); auto m02 = m(0, 2);
    auto m10 = m(1, 0); auto m11 = m(1, 1); auto m12 = m(1, 2);
    auto m20 = m(2, 0); auto m21 = m(2, 1); auto m22 = m(2, 2);
    // clang-format on

    constexpr auto ONE = static_cast<T>(1.0);
    constexpr auto TWO = static_cast<T>(2.0);
    constexpr auto HALF = static_cast<T>(0.5);
    constexpr auto QUARTER = static_cast<T>(0.25);

    auto trace = m00 + m11 + m22;
    if (trace > 0) {
        // The trace is valid, so we can do the algebra around solving for w
        auto s = HALF / std::sqrt(trace + ONE);

        m_Elements[0] = QUARTER / s;
        m_Elements[1] = (m21 - m12) * s;
        m_Elements[2] = (m02 - m20) * s;
        m_Elements[3] = (m10 - m01) * s;
    } else if ((m00 > m11) && (m00 > m22)) {
        // m00 is the dominant term, so do the algebra around solving for x
        auto s = TWO * std::sqrt(ONE + m00 - m11 - m22);

        m_Elements[0] = (m21 - m12) / s;
        m_Elements[1] = QUARTER * s;
        m_Elements[2] = (m01 + m10) / s;
        m_Elements[3] = (m02 + m20) / s;
    } else if (m11 > m22) {
        // m11 is the dominant term, so do the algebra around solving for y
        auto s = TWO * std::sqrt(ONE + m11 - m00 - m22);

        m_Elements[0] = (m02 - m20) / s;
        m_Elements[1] = (m01 + m10) / s;
        m_Elements[2] = QUARTER * s;
        m_Elements[3] = (m12 + m21) / s;
    } else {
        // m22 is the dominant term, so do the algebra around solving for z
        auto s = TWO * std::sqrt(ONE + m22 - m00 - m11);

        m_Elements[0] = (m10 - m01) / s;
        m_Elements[1] = (m02 + m20) / s;
        m_Elements[2] = (m12 + m21) / s;
        m_Elements[3] = QUARTER * s;
    }
}

template <typename T>
auto Quaternion<T>::setFromRotationMatrix(const Mat4& m) -> void {
    // clang-format off
    Mat3 mat_3(m(0, 0), m(0, 1), m(0, 2),
               m(1, 0), m(1, 1), m(1, 2),
               m(2, 0), m(2, 1), m(2, 2));
    // clang-format on
    setFromRotationMatrix(mat_3);
}

template <typename T>
auto Quaternion<T>::setFromEuler(const Euler<T>& euler) -> void {
    constexpr auto HALF = static_cast<T>(0.5);
    auto c1 = std::cos(HALF * euler.x);
    auto c2 = std::cos(HALF * euler.y);
    auto c3 = std::cos(HALF * euler.z);

    auto s1 = std::sin(HALF * euler.x);
    auto s2 = std::sin(HALF * euler.y);
    auto s3 = std::sin(HALF * euler.z);

    switch (euler.order) {
        case Euler<T>::Order::XYZ: {
            m_Elements[1] = s1 * c2 * c3 + c1 * s2 * s3;
            m_Elements[2] = c1 * s2 * c3 - s1 * c2 * s3;
            m_Elements[3] = c1 * c2 * s3 + s1 * s2 * c3;
            m_Elements[0] = c1 * c2 * c3 - s1 * s2 * s3;
            break;
        }
        case Euler<T>::Order::YXZ: {
            m_Elements[1] = s1 * c2 * c3 + c1 * s2 * s3;
            m_Elements[2] = c1 * s2 * c3 - s1 * c2 * s3;
            m_Elements[3] = c1 * c2 * s3 - s1 * s2 * c3;
            m_Elements[0] = c1 * c2 * c3 + s1 * s2 * s3;
            break;
        }
        case Euler<T>::Order::ZXY: {
            m_Elements[1] = s1 * c2 * c3 - c1 * s2 * s3;
            m_Elements[2] = c1 * s2 * c3 + s1 * c2 * s3;
            m_Elements[3] = c1 * c2 * s3 + s1 * s2 * c3;
            m_Elements[0] = c1 * c2 * c3 - s1 * s2 * s3;
            break;
        }
        case Euler<T>::Order::ZYX: {
            m_Elements[1] = s1 * c2 * c3 - c1 * s2 * s3;
            m_Elements[2] = c1 * s2 * c3 + s1 * c2 * s3;
            m_Elements[3] = c1 * c2 * s3 - s1 * s2 * c3;
            m_Elements[0] = c1 * c2 * c3 + s1 * s2 * s3;
            break;
        }
        case Euler<T>::Order::YZX: {
            m_Elements[1] = s1 * c2 * c3 + c1 * s2 * s3;
            m_Elements[2] = c1 * s2 * c3 + s1 * c2 * s3;
            m_Elements[3] = c1 * c2 * s3 - s1 * s2 * c3;
            m_Elements[0] = c1 * c2 * c3 - s1 * s2 * s3;
            break;
        }
        case Euler<T>::Order::XZY: {
            m_Elements[1] = s1 * c2 * c3 - c1 * s2 * s3;
            m_Elements[2] = c1 * s2 * c3 - s1 * c2 * s3;
            m_Elements[3] = c1 * c2 * s3 + s1 * s2 * c3;
            m_Elements[0] = c1 * c2 * c3 + s1 * s2 * s3;
            break;
        }
        default:
            break;
    }
}

template <typename T>
auto Quaternion<T>::setFromAxisAngle(const Vec3& axis, T angle) -> void {
    // Just in case, make sure the axis is normalized
    normalize_in_place<T>(axis);

    constexpr auto HALF = static_cast<T>(0.5);
    auto cos_half = std::cos(HALF * angle);
    auto sin_half = std::sin(HALF * angle);
    m_Elements[0] = cos_half;             // w
    m_Elements[1] = sin_half * axis.x();  // x
    m_Elements[2] = sin_half * axis.y();  // y
    m_Elements[3] = sin_half * axis.z();  // z
}

template <typename T>
auto Quaternion<T>::RotationX(T angle) -> Quaternion<T> {
    auto cos_half = std::cos(angle / static_cast<T>(2.0));
    auto sin_half = std::sin(angle / static_cast<T>(2.0));
    return Quaternion<T>(cos_half, sin_half, 0, 0);
}

template <typename T>
auto Quaternion<T>::RotationY(T angle) -> Quaternion<T> {
    auto cos_half = std::cos(angle / static_cast<T>(2.0));
    auto sin_half = std::sin(angle / static_cast<T>(2.0));
    return Quaternion<T>(cos_half, 0, sin_half, 0);
}

template <typename T>
auto Quaternion<T>::RotationZ(T angle) -> Quaternion<T> {
    auto cos_half = std::cos(angle / static_cast<T>(2.0));
    auto sin_half = std::sin(angle / static_cast<T>(2.0));
    return Quaternion<T>(cos_half, 0, 0, sin_half);
}

template <typename T>
auto Quaternion<T>::FromAxisAngle(Vector3<T> axis, T angle) -> Quaternion<T> {
    // Just in case, make sure the axis is normalized
    normalize_in_place<T>(axis);

    auto cos_half = std::cos(angle / static_cast<T>(2.0));
    auto sin_half = std::sin(angle / static_cast<T>(2.0));
    return Quaternion<T>(cos_half, sin_half * axis.x(), sin_half * axis.y(),
                         sin_half * axis.z());
}

template <typename T>
auto Quaternion<T>::FromRotationMatrix(Matrix3<T> matrix) -> Quaternion<T> {
    auto m00 = matrix(0, 0);
    auto m01 = matrix(0, 1);
    auto m02 = matrix(0, 2);

    auto m10 = matrix(1, 0);
    auto m11 = matrix(1, 1);
    auto m12 = matrix(1, 2);

    auto m20 = matrix(2, 0);
    auto m21 = matrix(2, 1);
    auto m22 = matrix(2, 2);

    Quaternion<T> quat;
    auto trace = m00 + m11 + m22;
    if (trace > 0) {
        // The trace is valid, so we can do the algebra around solving for w
        auto s = static_cast<T>(0.5) / std::sqrt(trace + static_cast<T>(1.0));

        quat.w() = static_cast<T>(0.25) / s;
        quat.x() = (m21 - m12) * s;
        quat.y() = (m02 - m20) * s;
        quat.z() = (m10 - m01) * s;
    } else if ((m00 > m11) && (m00 > m22)) {
        // m00 is the dominant term, so do the algebra around solving for x
        auto s = static_cast<T>(2.0) *
                 std::sqrt(static_cast<T>(1.0) + m00 - m11 - m22);

        quat.w() = (m21 - m12) / s;
        quat.x() = static_cast<T>(0.25) * s;
        quat.y() = (m01 + m10) / s;
        quat.z() = (m02 + m20) / s;
    } else if (m11 > m22) {
        // m11 is the dominant term, so do the algebra around solving for y
        auto s = static_cast<T>(2.0) *
                 std::sqrt(static_cast<T>(1.0) + m11 - m00 - m22);

        quat.w() = (m02 - m20) / s;
        quat.x() = (m01 + m10) / s;
        quat.y() = static_cast<T>(0.25) * s;
        quat.z() = (m12 + m21) / s;
    } else {
        // m22 is the dominant term, so do the algebra around solving for z
        auto s = static_cast<T>(2.0) *
                 std::sqrt(static_cast<T>(1.0) + m22 - m00 - m11);

        quat.w() = (m10 - m01) / s;
        quat.x() = (m02 + m20) / s;
        quat.y() = (m12 + m21) / s;
        quat.z() = static_cast<T>(0.25) * s;
    }

    return quat;
}

template <typename T>
auto Quaternion<T>::FromRotationMatrix(Matrix4<T> matrix) -> Quaternion<T> {
    auto m00 = matrix(0, 0);
    auto m01 = matrix(0, 1);
    auto m02 = matrix(0, 2);

    auto m10 = matrix(1, 0);
    auto m11 = matrix(1, 1);
    auto m12 = matrix(1, 2);

    auto m20 = matrix(2, 0);
    auto m21 = matrix(2, 1);
    auto m22 = matrix(2, 2);

    Quaternion<T> quat;
    auto trace = m00 + m11 + m22;
    if (trace > 0) {
        // The trace is valid, so we can do the algebra around solving for w
        auto s = static_cast<T>(0.5) / std::sqrt(trace + static_cast<T>(1.0));

        quat.w() = static_cast<T>(0.25) / s;
        quat.x() = (m21 - m12) * s;
        quat.y() = (m02 - m20) * s;
        quat.z() = (m10 - m01) * s;
    } else if ((m00 > m11) && (m00 > m22)) {
        // m00 is the dominant term, so do the algebra around solving for x
        auto s = static_cast<T>(2.0) *
                 std::sqrt(static_cast<T>(1.0) + m00 - m11 - m22);

        quat.w() = (m21 - m12) / s;
        quat.x() = static_cast<T>(0.25) * s;
        quat.y() = (m01 + m10) / s;
        quat.z() = (m02 + m20) / s;
    } else if (m11 > m22) {
        // m11 is the dominant term, so do the algebra around solving for y
        auto s = static_cast<T>(2.0) *
                 std::sqrt(static_cast<T>(1.0) + m11 - m00 - m22);

        quat.w() = (m02 - m20) / s;
        quat.x() = (m01 + m10) / s;
        quat.y() = static_cast<T>(0.25) * s;
        quat.z() = (m12 + m21) / s;
    } else {
        // m22 is the dominant term, so do the algebra around solving for z
        auto s = static_cast<T>(2.0) *
                 std::sqrt(static_cast<T>(1.0) + m22 - m00 - m11);

        quat.w() = (m10 - m01) / s;
        quat.x() = (m02 + m20) / s;
        quat.y() = (m12 + m21) / s;
        quat.z() = static_cast<T>(0.25) * s;
    }

    return quat;
}

// ***************************************************************************//
//                       Quaternion Methods and Operators                     //
// ***************************************************************************//

template <typename T>
using SFINAE_QUAT_GUARD = typename std::enable_if<IsScalar<T>::value>::type*;

/// Returns the square of the length of the given quaternion
template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto squareNorm(const Quaternion<T>& quat) -> T {
#if defined(MATH_AVX_ENABLED)
    return avx::kernel_length_square_quat<T>(quat.elements());
#elif defined(MATH_SSE_ENABLED)
    return sse::kernel_length_square_quat<T>(quat.elements());
#else
    return scalar::kernel_length_square_quat<T>(quat.elements());
#endif
}

/// Returns the length of the given quaternion
template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto norm(const Quaternion<T>& quat) -> T {
#if defined(MATH_AVX_ENABLED)
    return avx::kernel_length_quat<T>(quat.elements());
#elif defined(MATH_SSE_ENABLED)
    return sse::kernel_length_quat<T>(quat.elements());
#else
    return std::sqrt(scalar::kernel_length_square_quat<T>(quat.elements()));
#endif
}

/// Returns a normalized version of the given quaternion
template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto normalize(const Quaternion<T>& quat) -> Quaternion<T> {
    Quaternion<T> quat_normalized = quat;
#if defined(MATH_AVX_ENABLED)
    avx::kernel_normalize_in_place_quat<T>(quat_normalized.elements());
#elif defined(MATH_SSE_ENABLED)
    sse::kernel_normalize_in_place_quat<T>(quat_normalized.elements());
#else
    scalar::kernel_normalize_in_place_quat<T>(quat_normalized.elements());
#endif
    return quat_normalized;
}

/// Normalizes in place the given quaternion
template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto normalize_in_place(Quaternion<T>& quat) -> void {  // NOLINT
#if defined(MATH_AVX_ENABLED)
    avx::kernel_normalize_in_place_quat<T>(quat.elements());
#elif defined(MATH_SSE_ENABLED)
    sse::kernel_normalize_in_place_quat<T>(quat.elements());
#else
    scalar::kernel_normalize_in_place_quat<T>(quat.elements());
#endif
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator+(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    -> Quaternion<T> {
    Quaternion<T> dst;
#if defined(MATH_AVX_ENABLED)
    avx::kernel_add_quat<T>(dst.elements(), lhs.elements(), rhs.elements());
#elif defined(MATH_SSE_ENABLED)
    sse::kernel_add_quat<T>(dst.elements(), lhs.elements(), rhs.elements());
#else
    scalar::kernel_add_quat<T>(dst.elements(), lhs.elements(), rhs.elements());
#endif
    return dst;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator-(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    -> Quaternion<T> {
    Quaternion<T> dst;
#if defined(MATH_AVX_ENABLED)
    avx::kernel_sub_quat<T>(dst.elements(), lhs.elements(), rhs.elements());
#elif defined(MATH_SSE_ENABLED)
    sse::kernel_sub_quat<T>(dst.elements(), lhs.elements(), rhs.elements());
#else
    scalar::kernel_sub_quat<T>(dst.elements(), lhs.elements(), rhs.elements());
#endif
    return dst;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator*(double scale, const Quaternion<T>& quat)
    -> Quaternion<T> {
    Quaternion<T> dst;
#if defined(MATH_AVX_ENABLED)
    avx::kernel_scale_quat<T>(dst.elements(), static_cast<T>(scale),
                              quat.elements());
#elif defined(MATH_SSE_ENABLED)
    sse::kernel_scale_quat<T>(dst.elements(), static_cast<T>(scale),
                              quat.elements());
#else
    scalar::kernel_scale_quat<T>(dst.elements(), static_cast<T>(scale),
                                 quat.elements());
#endif
    return dst;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator*(const Quaternion<T>& quat, double scale)
    -> Quaternion<T> {
    Quaternion<T> dst;
#if defined(MATH_AVX_ENABLED)
    avx::kernel_scale_quat<T>(dst.elements(), static_cast<T>(scale),
                              quat.elements());
#elif defined(MATH_SSE_ENABLED)
    sse::kernel_scale_quat<T>(dst.elements(), static_cast<T>(scale),
                              quat.elements());
#else
    scalar::kernel_scale_quat<T>(dst.elements(), static_cast<T>(scale),
                                 quat.elements());
#endif
    return dst;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    -> Quaternion<T> {
    Quaternion<T> dst;
    scalar::kernel_quatmul_quat<T>(dst.elements(), lhs.elements(),
                                   rhs.elements());
    return dst;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto conjugate(const Quaternion<T>& quat) -> Quaternion<T> {
    Quaternion<T> dst = quat;
    dst.x() = -dst.x();
    dst.y() = -dst.y();
    dst.z() = -dst.z();
    return dst;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto inverse(const Quaternion<T>& quat) -> Quaternion<T> {
    auto q_conj = conjugate<T>(quat);
    auto length_sq = squareNorm<T>(quat);
    auto q_inv = q_conj * (1.0 / static_cast<double>(length_sq));
    return q_inv;
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto rotate(const Quaternion<T>& quat, const Vector3<T>& vec)
    -> Vector3<T> {
    // We use the form f(p) = q * p * q ^-1
    Quaternion<T> quat_p(static_cast<T>(0.0), vec.x(), vec.y(), vec.z());
    auto quat_qinv = inverse(quat);
    auto quat_qp = quat * quat_p;
    auto quat_qpqinv = quat_qp * quat_qinv;
    return Vector3<T>(quat_qpqinv.x(), quat_qpqinv.y(), quat_qpqinv.z());
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    -> bool {
    return scalar::kernel_compare_eq_quat<T>(lhs.elements(), rhs.elements());
}

template <typename T, SFINAE_QUAT_GUARD<T> = nullptr>
LM_INLINE auto operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    -> bool {
    return !scalar::kernel_compare_eq_quat<T>(lhs.elements(), rhs.elements());
}

}  // namespace math
