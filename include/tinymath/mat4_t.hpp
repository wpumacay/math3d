#pragma once

#include <string>
#include <tinymath/vec4_t.hpp>

namespace tiny {
namespace math {

/// \class Matrix4
///
/// \brief Class representation of a 4 by 4 matrix of real-valued entries
///
/// \tparam Scalar_T Type of scalar value used for the entries of the matrix
///
/// This is a class that represents 4x4 matrices with real-valued entries. The
/// internal data is stored as the columns of the matrix using 4d vectors of the
/// same scalar type. The resulting storage is column major and aligned in a way
/// that allows the use of aligned versions of some SIMD instructions (when
/// using either SSE or AVX instrinsics).
template <typename Scalar_T>
class Matrix4 {
 public:
    /// Number of scalars used in the storage of the matrix
    constexpr static uint32_t BUFFER_SIZE = 16;
    /// Number of dimensions of the matrix (square 4x4 matrix)
    constexpr static uint32_t MATRIX_NDIM = 4;

    /// Typename of the matrix
    using Type = Matrix4<Scalar_T>;
    /// Typename of the scalar used for the matrix entries (float, double, etc.)
    using ElementType = Scalar_T;
    /// Typename of the internal storage used by the matrix
    using BufferType = std::array<Vector4<Scalar_T>, MATRIX_NDIM>;
    /// Typename of the columns of the matrix
    using ColumnType = Vector4<Scalar_T>;

    /// Creates a zero-initialized matrix
    Matrix4() = default;

    /// Creates a matrix using the given scalars for its entries
    explicit Matrix4(Scalar_T x00, Scalar_T x01, Scalar_T x02, Scalar_T x03,
                     Scalar_T x10, Scalar_T x11, Scalar_T x12, Scalar_T x13,
                     Scalar_T x20, Scalar_T x21, Scalar_T x22, Scalar_T x23,
                     Scalar_T x30, Scalar_T x31, Scalar_T x32, Scalar_T x33);

    /// Creates a diagonal matrix using the given diagonal elements
    explicit Matrix4(Scalar_T d0, Scalar_T d1, Scalar_T d2, Scalar_T d3);

    /// Creates a matrix using the given columns
    explicit Matrix4(const ColumnType& col0, const ColumnType& col1,
                     const ColumnType& col2, const ColumnType& col3);

    /// Returns a mutable reference to the underlying storage of the matrix
    auto elements() -> BufferType& { return m_Elements; }

    /// Returns an unmutable reference to the underlying storage of the matrix
    auto elements() const -> const BufferType& { return m_Elements; }

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

    /// Transposes the matrix in-place
    TM_INLINE auto transposeInPlace() -> void;

    /// Returns the transposed version of the matrix
    TM_INLINE auto transpose() const -> Matrix4<Scalar_T>;

    /// Returns a printable string-representation of the matrix
    auto toString() const -> std::string;

    /// Returns the number of rows
    constexpr auto rows() const -> uint32_t { return MATRIX_NDIM; }

    /// Returns the number of columns
    constexpr auto cols() const -> uint32_t { return MATRIX_NDIM; }

    /// Returns the number of elements in the matrix
    constexpr auto size() const -> uint32_t {
        return MATRIX_NDIM * MATRIX_NDIM;
    }

    /// Returns the dimnension of the matrix
    constexpr auto ndim() const -> uint32_t { return MATRIX_NDIM; }

    /// Returns the number of scalars used by the storage of the matrix
    constexpr auto buffer_size() const -> uint32_t { return BUFFER_SIZE; }

    /// Returns the size (in bytes) of the matrix
    constexpr auto num_bytes_size() const -> uint32_t { return sizeof(Type); }

    /// Returns the alignment (in bytes) of the matrix
    constexpr auto num_bytes_alignment() const -> uint32_t {
        return alignof(Type);
    }

    /// Returns a 4x4 identity matrix of the current scalar type
    static auto Identity() -> Type;

    /// Returns a 4x4 zero matrix of the current scalar type
    static auto Zeros() -> Type;

 private:
    /// The buffer where all data lies (as an array of 4 column vectors)
    alignas(sizeof(Scalar_T) * BUFFER_SIZE) BufferType m_Elements;
};

}  // namespace math
}  // namespace tiny