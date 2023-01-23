#pragma once

#include <iomanip>
#include <ios>
#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>

#include <math/vec3_t_decl.hpp>
#include <math/quat_t_decl.hpp>

namespace math {

template <typename Scalar_T>
class Quaternion;

}  // namespace math

namespace math {

/// \class Matrix3
///
/// \brief Class representation of a 3 by 3 matrix of real-valued entries
///
/// \tparam Scalar_T Type of scalar value used for the entries of the matrix
///
/// This is a class that represents 3x3 matrices with real-valued entries. The
/// internal data is stored as the columns of the matrix using 3d vectors of the
/// same scalar type, thus using a column major order.
template <typename Scalar_T>
class Matrix3 {
 public:
    /// Number of scalars used for the storage of this matrix
    static constexpr uint32_t BUFFER_SIZE = 9;
    /// Number of dimensions of the matrix (square 3x3 matrix)
    static constexpr uint32_t MATRIX_SIZE = 3;
    /// Number of dimensions of this matrix (as in numpy.ndarray.ndim)
    static constexpr uint32_t MATRIX_NDIM = 2;

    /// Typename of the matrix
    using Type = Matrix3<Scalar_T>;
    /// Typename of the scalar used for the matrix entries (float, double, etc.)
    using ElementType = Scalar_T;
    /// Typename of the internal storage used by the matrix
    using BufferType = std::array<Vector3<Scalar_T>, MATRIX_SIZE>;
    /// Typename of the columns of the matrix
    using ColumnType = Vector3<Scalar_T>;

    /// Creates a zero-initialized matrix
    Matrix3() = default;

    // clang-format off
    /// Creates a matrix using the given scalars for its entries
    explicit Matrix3(Scalar_T x00, Scalar_T x01, Scalar_T x02,
                     Scalar_T x10, Scalar_T x11, Scalar_T x12,
                     Scalar_T x20, Scalar_T x21, Scalar_T x22) {
        m_Elements[0][0] = x00;
        m_Elements[1][0] = x01;
        m_Elements[2][0] = x02;

        m_Elements[0][1] = x10;
        m_Elements[1][1] = x11;
        m_Elements[2][1] = x12;

        m_Elements[0][2] = x20;
        m_Elements[1][2] = x21;
        m_Elements[2][2] = x22;
    }
    // clang-format on

    explicit Matrix3(Scalar_T x00, Scalar_T x11, Scalar_T x22) {
        m_Elements[0][0] = x00;
        m_Elements[1][1] = x11;
        m_Elements[2][2] = x22;
    }

    explicit Matrix3(const ColumnType& col0, const ColumnType& col1,
                     const ColumnType& col2) {
        m_Elements[0] = col0;
        m_Elements[1] = col1;
        m_Elements[2] = col2;
    }

    /// Returns a mutable reference to the underlying storage of the matrix
    auto elements() -> BufferType& { return m_Elements; }

    /// Returns an unmutable reference to the underlying storage of the matrix
    auto elements() const -> const BufferType& { return m_Elements; }

    /// Returns a pointer to the data of the underlying storage in use
    auto data() -> Scalar_T* { return m_Elements[0].data(); }

    /// Reeturns a const-pointer to the data of the underlying storage in use
    auto data() const -> const Scalar_T* { return m_Elements[0].data(); }

    /// Gets a mutable reference to the column requested by the given index
    auto operator[](uint32_t col_index) -> ColumnType& {
        return m_Elements[col_index];
    }

    /// Gets an unmutable reference to the column requested by the given index
    auto operator[](uint32_t col_index) const -> const ColumnType& {
        return m_Elements[col_index];
    }

    /// Gets a mutable reference to the requested matrix entry
    auto operator()(uint32_t row_index, uint32_t col_index) -> Scalar_T& {
        return m_Elements[col_index][row_index];
    }

    /// Gets an unmutable reference to the requested matrix entry
    auto operator()(uint32_t row_index, uint32_t col_index) const
        -> const Scalar_T& {
        return m_Elements[col_index][row_index];
    }

    /// Returns a comma-initializer to construct the matrix via its coefficients
    auto operator<<(Scalar_T coeff) -> MatCommaInitializer<Type> {
        return MatCommaInitializer<Type>(*this, coeff);
    }

    /// Returns a printable string-representation of the matrix
    auto toString() const -> std::string {
        const auto& data_elms = m_Elements;
        std::stringstream sstr_result;

        sstr_result << "( " << data_elms[0][0] << ", " << data_elms[1][0]
                    << ", " << data_elms[2][0] << ",\n";
        sstr_result << "  " << data_elms[0][1] << ", " << data_elms[1][1]
                    << ", " << data_elms[2][1] << ",\n";
        sstr_result << "  " << data_elms[0][2] << ", " << data_elms[1][2]
                    << ", " << data_elms[2][2] << ')';

        return sstr_result.str();
    }

    /// Creates a rotation matrix from the given quaternion
    static auto FromQuaternion(Quaternion<Scalar_T> quat) -> Matrix3<Scalar_T>;

    /// Creates a rotation matrix for the given angle around the X-axis
    static auto RotationX(Scalar_T angle) -> Matrix3<Scalar_T>;

    /// Creates a rotation matrix for the given angle around the Y-axis
    static auto RotationY(Scalar_T angle) -> Matrix3<Scalar_T>;

    /// Creates a rotation matrix for the given angle around the Z-axis
    static auto RotationZ(Scalar_T angle) -> Matrix3<Scalar_T>;

    /// Creates a scale matrix for the given separate scale arguments
    static auto Scale(Scalar_T scale_x, Scalar_T scale_y, Scalar_T scale_z)
        -> Matrix3<Scalar_T>;

    /// Creates a scale matrix for the given scale arguments given as a vec-3
    static auto Scale(const Vector3<Scalar_T>& scale) -> Matrix3<Scalar_T>;

    /// Creates an Identity matrix
    static auto Identity() -> Matrix3<Scalar_T>;

    /// Creates a Zero matrix
    static auto Zeros() -> Matrix3<Scalar_T>;

    /// Returns the number of rows
    static constexpr auto rows() -> uint32_t { return MATRIX_SIZE; }

    /// Returns the number of columns
    static constexpr auto cols() -> uint32_t { return MATRIX_SIZE; }

    /// Returns the number of elements in the matrix
    static constexpr auto size() -> uint32_t {
        return MATRIX_SIZE * MATRIX_SIZE;
    }

    /// Returns the dimnension of the matrix
    static constexpr auto ndim() -> uint32_t { return MATRIX_SIZE; }

    /// Returns the number of scalars used by the storage of the matrix
    static constexpr auto buffer_size() -> uint32_t { return BUFFER_SIZE; }

    /// Returns the size (in bytes) of the matrix
    static constexpr auto num_bytes_size() -> uint32_t { return sizeof(Type); }

    /// Returns the alignment (in bytes) of the matrix
    static constexpr auto num_bytes_alignment() -> uint32_t {
        return alignof(Type);
    }

 private:
    /// The buffer where all data is stored
    BufferType m_Elements;
};

}  // namespace math