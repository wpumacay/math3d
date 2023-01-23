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

#include <math/common.hpp>

namespace math {

/// \class Vector3
///
/// \brief Class representation of a vector in 3d-space
///
/// \tparam Scalar_T Type of scalar value used for this 3d-vector (float|double)
///
/// This is a class that represents a 3d-vector with entries x, y, z of some
/// scalar floating-point type. Its storage is a buffer of the given scalar
/// type, and contains only the required storage for 3 elements.
template <typename Scalar_T>
class Vector3 {
 public:
    /// Number of scalars used in the storage of the vector
    constexpr static uint32_t BUFFER_SIZE = 3;
    /// Number of scalar dimensions of the vector
    constexpr static uint32_t VECTOR_SIZE = 3;
    /// Number of dimensions of this vector (as in np.array.ndim)
    static constexpr uint32_t VECTOR_NDIM = 1;

    /// Typename of the vector
    using Type = Vector3<Scalar_T>;
    /// Typename of the scalar used for the vector (float32, float64, etc.)
    using ElementType = Scalar_T;
    /// Typename of the internal storage used for the vector
    using BufferType = std::array<Scalar_T, BUFFER_SIZE>;

    /// Constructs a zero-initialized vector
    Vector3() = default;

    /// Constructs a vector of the form (x, x, x)
    explicit Vector3(Scalar_T x_coord) {
        m_Elements[0] = x_coord;
        m_Elements[1] = x_coord;
        m_Elements[2] = x_coord;
    }

    /// Constructs a vector of the form (x, y, y)
    explicit Vector3(Scalar_T x_coord, Scalar_T y_coord) {
        m_Elements[0] = x_coord;
        m_Elements[1] = y_coord;
        m_Elements[2] = y_coord;
    }

    /// Constructs a vector of the form (x, y, z)
    explicit Vector3(Scalar_T x_coord, Scalar_T y_coord, Scalar_T z_coord) {
        m_Elements[0] = x_coord;
        m_Elements[1] = y_coord;
        m_Elements[2] = z_coord;
    }

    // cppcheck-suppress noExplicitConstructor
    /// COnstructs a vector from an initializer list of the form {x, y, z}
    Vector3(const std::initializer_list<Scalar_T>& values) {
        // Complain in case we don't receive exactly 3 values
        assert(values.size() == Vector3<Scalar_T>::VECTOR_SIZE);
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
    BufferType m_Elements = {0, 0, 0};
};

}  // namespace math