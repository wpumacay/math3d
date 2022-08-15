#pragma once

// clang-format off
#include <ios>
#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <type_traits>

#include <loco/math/common.hpp>
// clang-format on

namespace loco {
namespace math {

/// \class Vector3
///
/// \brief Class representation of a vector in 3d-space
///
/// \tparam Scalar_T Type of scalar value used for this 3d-vector (float|double)
///
/// This is a class that represents a 3d-vector with entries x, y, z of some
/// scalar floating-point type. Its storage is a buffer of the given scalar
/// type, and it's aligned accordingly in order to use the aligned versions of
/// some SIMD instructions (when using either SSE or AVX intrinsics).
template <typename Scalar_T>
class Vector3 {
 public:
    /// Number of scalars used in the storage of the vector
    constexpr static uint32_t BUFFER_SIZE = 4;
    /// Number of scalar dimensions of the vector
    constexpr static uint32_t VECTOR_NDIM = 3;

    /// Typename of the vector
    using Type = Vector3<Scalar_T>;
    /// Typename of the scalar used for the vector (float32, float64, etc.)
    using ElementType = Scalar_T;
    /// Typename of the internal storage used for the vector
    using BufferType = std::array<Scalar_T, BUFFER_SIZE>;

    /// Constructs a zero-initialized vector
    Vector3() = default;

    /// Constructs a vector of the form (x, x, x)
    explicit Vector3(Scalar_T x) {
        m_Elements[0] = x;
        m_Elements[1] = x;
        m_Elements[2] = x;
        m_Elements[3] = 0;
    }

    /// Constructs a vector of the form (x, y, y)
    explicit Vector3(Scalar_T x, Scalar_T y) {
        m_Elements[0] = x;
        m_Elements[1] = y;
        m_Elements[2] = y;
        m_Elements[3] = 0;
    }

    /// Constructs a vector of the form (x, y, z)
    explicit Vector3(Scalar_T x, Scalar_T y, Scalar_T z) {
        m_Elements[0] = x;
        m_Elements[1] = y;
        m_Elements[2] = z;
        m_Elements[3] = 0;
    }

    // cppcheck-suppress noExplicitConstructor
    /// COnstructs a vector from an initializer list of the form {x, y, z}
    Vector3(const std::initializer_list<Scalar_T>& values) {
        // Complain in case we don't receive exactly 3 values
        assert(values.size() == Vector3<Scalar_T>::VECTOR_NDIM);
        // Just copy the whole data from the initializer list
        std::copy(values.begin(), values.end(), m_Elements.data());
    }

    /// Returns a mutable reference to the x-component of the vector
    auto x() -> Scalar_T& { return m_Elements[0]; }

    /// Returns a mutable reference to the y-component of the vector
    auto y() -> Scalar_T& { return m_Elements[1]; }

    /// Returns a mutable reference to the z-component of the vector
    auto z() -> Scalar_T& { return m_Elements[2]; }

    /// Returns an unmutable reference to the x-component of the vector
    auto x() const -> const Scalar_T& { return m_Elements[0]; }

    /// Returns an unmutable reference to the y-component of the vector
    auto y() const -> const Scalar_T& { return m_Elements[1]; }

    /// Returns an unmutable reference to the z-component of the vector
    auto z() const -> const Scalar_T& { return m_Elements[2]; }

    /// Returns a mutable reference to the underlying storage of the vector
    auto elements() -> BufferType& { return m_Elements; }

    /// Returns an unmutable reference to the underlying storage of the vector
    auto elements() const -> const BufferType& { return m_Elements; }

    /// Returns a pointer to the data of the underlying storage in use
    auto data() -> Scalar_T* { return m_Elements.data(); }

    /// Reeturns a const-pointer to the data of the underlying storage in use
    auto data() const -> const Scalar_T* { return m_Elements.data(); }

    /// Returns a mutable reference to the requested entry of the vector
    auto operator[](uint32_t index) -> Scalar_T& { return m_Elements[index]; }

    /// Returns an unmutable reference to the requested entry of the vector
    auto operator[](uint32_t index) const -> const Scalar_T& {
        return m_Elements[index];
    }

    /// Returns a comma-initializer to construct the vector via its coefficients
    auto operator<<(Scalar_T coeff) -> VecCommaInitializer<Type> {
        return VecCommaInitializer<Type>(*this, coeff);
    }

    /// Returns a printable string-representation of the vector
    auto toString() const -> std::string {
        std::stringstream str_result;
        if (std::is_same<ElementType, float>()) {
            str_result << "Vector3f(" << x() << ", " << y() << ", " << z()
                       << ")";
        } else if (std::is_same<ElementType, double>()) {
            str_result << "Vector3d(" << x() << ", " << y() << ", " << z()
                       << ")";
        } else {
            str_result << "Vector3X(" << x() << ", " << y() << ", " << z()
                       << ")";
        }
        return str_result.str();
    }

    /// Returns the number of dimensions of the vector (Vector3 <-> 3 scalars)
    static constexpr auto ndim() -> uint32_t { return VECTOR_NDIM; }

    /// Returns the number of scalars used by the storage of the vector
    static constexpr auto buffer_size() -> uint32_t { return BUFFER_SIZE; }

    /// Returns the size (in bytes) of the vector
    static constexpr auto num_bytes_size() -> uint32_t { return sizeof(Type); }

    /// Returns the alignment (in bytes) of the vector
    static constexpr auto num_bytes_alignment() -> uint32_t {
        return alignof(Type);
    }

 private:
    /// Storage of the vector's scalars (we pad by 1 due to SIMD-alignment)
    alignas(sizeof(Scalar_T) * BUFFER_SIZE) BufferType m_Elements = {0, 0, 0,
                                                                     0};
};

}  // namespace math
}  // namespace loco