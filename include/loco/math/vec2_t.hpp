#pragma once

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

namespace loco {
namespace math {

/// \class Vector2
///
/// \brief Class representation of a vector in 2d-space
///
/// \tparam Scalar_T Type of scalar value used for this 2d-vector (float|double)
///
/// This is a class that represents a 2d-vector with entries x, y of some
/// scalar floating-point type. Its storage is a buffer of the given scalar
/// type; however, it's not aligned for SIMD load/store instructions (tradeoff
/// between adding padding/size and usage of aligned load/store). So, SIMD
/// kernels are build using unaligned load/store operations (unlike friends like
/// vec3 and vec4 types)
template <typename Scalar_T>
struct Vector2 {
    /// Number of scalars used in the storage of the vector
    static constexpr uint32_t BUFFER_SIZE = 2;
    /// Number of scalars contained by the vector
    static constexpr uint32_t VECTOR_SIZE = 2;
    /// Number of dimensions of this vector (as in numpy.ndarray.ndim)
    static constexpr uint32_t VECTOR_NDIM = 1;

    /// Type alias of the vector
    using Type = Vector2<Scalar_T>;
    /// Type alias of the scalar used for this vector (float32|64)
    using ElementType = Scalar_T;
    /// Type alias of the internal storage used for the vector (i.e. std::array)
    using BufferType = std::array<Scalar_T, BUFFER_SIZE>;

    /// Constructs a zero-initialized vector
    Vector2() = default;

    /// Constructs a vector of the form (x, x)
    explicit Vector2(Scalar_T x_coord) {
        m_Elements[0] = x_coord;
        m_Elements[1] = x_coord;
    }

    /// Constructs a vector of the form (x, y)
    explicit Vector2(Scalar_T x_coord, Scalar_T y_coord) {
        m_Elements[0] = x_coord;
        m_Elements[1] = y_coord;
    }

    // cppcheck-suppress noExplicitConstructor
    /// Constructs a vector from an initializer list of the form {x, y}
    Vector2(const std::initializer_list<Scalar_T>& values) {
        assert(values.size() == Vector2<Scalar_T>::VECTOR_SIZE);
        std::copy(values.begin(), values.end(), m_Elements.data());
    }

    /// Returns a mutable reference to the x-component of the vector
    auto x() -> Scalar_T& { return m_Elements[0]; }

    /// Returns a mutable reference to the y-component of the vector
    auto y() -> Scalar_T& { return m_Elements[1]; }

    /// Returns an unmutable reference to the x-component of the vector
    auto x() const -> const Scalar_T& { return m_Elements[0]; }

    /// Returns an unmutable reference to the y-component of the vector
    auto y() const -> const Scalar_T& { return m_Elements[1]; }

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
            str_result << "Vector2f(" << x() << ", " << y() << ")";
        } else if (std::is_same<ElementType, double>()) {
            str_result << "Vector2d(" << x() << ", " << y() << ")";
        } else {
            str_result << "Vector2X(" << x() << ", " << y() << ")";
        }
        return str_result.str();
    }

    /// Returns the number of dimensions of the vector (Vector2 <-> 2 scalars)
    static constexpr auto size() -> uint32_t { return VECTOR_SIZE; }

    /// Returns the number of scalars used by the storage of the vector
    static constexpr auto buffer_size() -> uint32_t { return BUFFER_SIZE; }

    /// Returns the size (in bytes) of the vector
    static constexpr auto num_bytes_size() -> uint32_t { return sizeof(Type); }

    /// Returns the alignment (in bytes) of the vector
    static constexpr auto num_bytes_alignment() -> uint32_t {
        return alignof(Type);
    }

 private:
    /// Storage of the vector's scalars
    BufferType m_Elements = {0, 0};
};

}  // namespace math
}  // namespace loco
