#pragma once

// -----------------------------------------------------------------------------
// Language detection adapted from https://github.com/g-truc/glm

// clang-format off

#if defined(MATH3D_FORCE_INLINE)
    #if defined(MATH3D_COMPILER_CLANG) || defined(MATH3D_COMPILER_GCC)
        #define MATH3D_INLINE inline __attribute__((__always_inline__))
    #elif defined(MATH3D_COMPILER_MSVC)
        #define MATH3D_INLINE __forceinline
    #else
        #define MATH3D_INLINE inline
    #endif
#else
    #define MATH3D_INLINE
#endif

#if defined(MATH3D_COMPILER_CLANG) || defined(MATH3D_COMPILER_GCC)
    #define MATH3D_NEVER_INLINE __attribute__((noinline))
#elif defined(MATH3D_COMPILER_MSVC)
    #define MATH3D_NEVER_INLINE __declspec(noinline)
#else
    #define MATH3D_NEVER_INLINE
#endif

#define MATH3D_LANG_CXX98_FLAG (1 << 1)
#define MATH3D_LANG_CXX03_FLAG (1 << 2)
#define MATH3D_LANG_CXX0X_FLAG (1 << 3)
#define MATH3D_LANG_CXX11_FLAG (1 << 4)
#define MATH3D_LANG_CXX14_FLAG (1 << 5)
#define MATH3D_LANG_CXX17_FLAG (1 << 6)
#define MATH3D_LANG_CXX20_FLAG (1 << 7)

#define MATH3D_LANG_CXX98 MATH3D_LANG_CXX98_FLAG
#define MATH3D_LANG_CXX03 (MATH3D_LANG_CXX98 | MATH3D_LANG_CXX03_FLAG)
#define MATH3D_LANG_CXX0X (MATH3D_LANG_CXX03 | MATH3D_LANG_CXX0X_FLAG)
#define MATH3D_LANG_CXX11 (MATH3D_LANG_CXX0X | MATH3D_LANG_CXX11_FLAG)
#define MATH3D_LANG_CXX14 (MATH3D_LANG_CXX11 | MATH3D_LANG_CXX14_FLAG)
#define MATH3D_LANG_CXX17 (MATH3D_LANG_CXX14 | MATH3D_LANG_CXX17_FLAG)
#define MATH3D_LANG_CXX20 (MATH3D_LANG_CXX17 | MATH3D_LANG_CXX20_FLAG)

#if defined(MATH3D_FORCE_CXX20)
    #define MATH3D_LANG MATH3D_LANG_CXX20
#elif defined(MATH3D_FORCE_CXX17)
    #define MATH3D_LANG MATH3D_LANG_CXX17
#elif defined(MATH3D_FORCE_CXX14)
    #define MATH3D_LANG MATH3D_LANG_CXX14
#elif defined(MATH3D_FORCE_CXX11)
    #define MATH3D_LANG MATH3D_LANG_CXX11
#else
    #if __cplusplus > 201703L
        #define MATH3D_LANG MATH3D_LANG_CXX20
    #elif __cplusplus == 201703L
        #define MATH3D_LANG MATH3D_LANG_CXX17
    #elif __cplusplus == 201402L
        #define MATH3D_LANG MATH3D_LANG_CXX14
    #elif __cplusplus == 201103L
        #define MATH3D_LANG MATH3D_LANG_CXX11
    #else
        #error "C++ standard must be one of 11, 14, 17, and 20"
    #endif
#endif

// [[nodiscard]]
#if MATH3D_LANG & MATH3D_LANG_CXX17_FLAG
    #define MATH3D_NODISCARD [[nodiscard]]
#else
    #define MATH3D_NODISCARD
#endif

#if defined _WIN32 || defined __CYGWIN__
    #define MATH3D_DLL_EXPORT __declspec(dllexport)
    #define MATH3D_DLL_IMPORT __declspec(dllimport)
    #define MATH3D_DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define MATH3D_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define MATH3D_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define MATH3D_DLL_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define MATH3D_DLL_EXPORT
        #define MATH3D_DLL_IMPORT
        #define MATH3D_DLL_LOCAL
    #endif
#endif


#define MATH3D_DECL MATH3D_NODISCARD

#ifdef MATH3D_STATIC
    #define MATH3D_API
    #define MATH3D_LOCAL
#else
    #ifdef MATH3D_DLL_EXPORTS
        #define MATH3D_API MATH3D_DLL_EXPORT
    #else
        #define MATH3D_API MATH3D_DLL_IMPORT
    #endif
    #define MATH3D_LOCAL MATH3D_DLL_LOCAL
#endif

// clang-format on

// -----------------------------------------------------------------------------

#include <cassert>
#include <cstdint>
#include <algorithm>
#include <type_traits>

namespace math {

static constexpr double PI = 3.141592653589793;  // NOLINT
static constexpr double EPS = 1e-6;

using float32_t = float;
using float64_t = double;
using uint = unsigned int;

template <uint z, uint y, uint x, uint w>
struct ShuffleMask {
    // NOLINTNEXTLINE
    static constexpr uint value = (((z) << 6) | ((y) << 4) | ((x) << 2) | (w));
};

#if defined(MATH3D_SSE_ENABLED)
using HAS_SSE = std::true_type;
#else
using HAS_SSE = std::false_type;
#endif

#if defined(MATH3D_AVX_ENABLED)
using HAS_AVX = std::true_type;
#else
using HAS_AVX = std::false_type;
#endif

// clang-format off
template <typename Tp> struct IsFloat32 : public std::false_type {};
template <> struct IsFloat32<float32_t> : public std::true_type {};

template <typename Tp> struct IsFloat64 : public std::false_type {};
template <> struct IsFloat64<float64_t> : public std::true_type {};

template <typename Tp>
struct IsScalar : public std::integral_constant<bool,
                IsFloat32<Tp>::value || IsFloat64<Tp>::value> {};

template <typename Tp>
struct CpuNoSIMD : public std::integral_constant<bool,
                IsScalar<Tp>::value && !HAS_SSE::value && !HAS_AVX::value> {};

template <typename Tp>
struct CpuHasSIMD : public std::integral_constant<bool,
                IsScalar<Tp>::value && (HAS_SSE::value || HAS_AVX::value)> {};

template <typename Tp>
struct CpuHasSSE : public std::integral_constant<bool,
                IsScalar<Tp>::value && HAS_SSE::value> {};

template <typename Tp>
struct CpuHasAVX : public std::integral_constant<bool,
                IsScalar<Tp>::value && HAS_AVX::value> {};

// clang-format on

/// \class VecCommaInitializer
///
/// \brief Helper class used during comma-initialization of vec-types
///
/// \tparam V Type of vector related to this comma initializer
///
/// This is a helper class used for operations of the form `v << 1, 2, 3, ...;`,
/// which require to concatenate a comma-initializer after using the `<<`
/// operator. This is based on Eigen's comma-initializer implementation for
/// vector types.
///
/// \code
///     Vector3d vec;
///     vec << 1.0, 2.0, 3.0;
/// \endcode
template <typename V>
struct VecCommaInitializer {
    /// Number of scalar dimensions of the vector
    static constexpr uint32_t VECTOR_SIZE = V::VECTOR_SIZE;
    /// Index of the first vector entry on its storage buffer|array
    static constexpr uint32_t VECTOR_FIRST_INDEX = 0;
    /// Index of the last vector entry on its storage buffer|array
    static constexpr uint32_t VECTOR_LAST_INDEX = VECTOR_SIZE - 1;

    /// Type alias for the float|scalar type in use
    using T = typename V::ElementType;
    /// Type alias for this comma-initializer
    using Type = VecCommaInitializer<V>;
    /// Type alias for the vector type linked to this initializer
    using VectorType = V;

    /// Creates a comma-initializer for the given vector and initial coeff.
    // NOLINTNEXTLINE(runtime/references)
    explicit VecCommaInitializer(VectorType& vec, const T& coeff0)
        : m_VectorRef(vec) {
        // Append first coefficient to the vector
        m_VectorRef[m_CurrentBuildIndex++] = coeff0;
    }

    // Follow RAII and the 'Rule of 5' (use defaults though) -------------------

    VecCommaInitializer(const VecCommaInitializer<V>& rhs) = default;

    VecCommaInitializer(VecCommaInitializer<V>&& rhs) noexcept = default;

    auto operator=(const VecCommaInitializer<V>& rhs)
        -> VecCommaInitializer<V>& = default;

    auto operator=(VecCommaInitializer<V>&& rhs) noexcept
        -> VecCommaInitializer<V>& = default;

    /// Release any resources and terminate the initializer's operation
    ~VecCommaInitializer() { _finished(); }

    // -------------------------------------------------------------------------

    /// Appends the given coefficient to the vector managed by this initializer
    auto operator,(const T& next_coeff) -> Type& {
        assert(m_CurrentBuildIndex <= VECTOR_LAST_INDEX);
        m_VectorRef[m_CurrentBuildIndex++] = next_coeff;
        return *this;
    }

 private:
    /// Terminates the operations of the initializer
    MATH3D_INLINE auto _finished() -> void {
        assert(m_CurrentBuildIndex == (VECTOR_LAST_INDEX + 1));
    }

 private:
    /// Mutable reference to the vector we're currently constructing
    VectorType& m_VectorRef;  // NOLINT
    /// Index of the current coefficient being 'built'
    uint32_t m_CurrentBuildIndex = VECTOR_FIRST_INDEX;
};

/// \class MatCommaInitializer
///
/// \brief Helper class used during comma-initialization of matrix-types
///
/// \tparam V Type of matrix related to this comma initializer
///
/// This is a helper class used for operations of the form `m << 1.0, 2.0,...;`,
/// which require to concatenate a comma-initializer after using the `<<`
/// operator. This is based on Eigen's comma-initializer implementation for
/// matrix types.
///
/// \code
///     Matrix4d mat;
///     mat << 1.0,  2.0,  3.0,  4.0,
///            5.0,  6.0,  7.0,  8.0,
///            9.0,  10.0, 11.0, 12.0,
///            13.0, 14.0, 15.0, 16.0;
/// \endcode
template <typename M>
struct MatCommaInitializer {
    /// Number of scalar dimensions of the vector
    constexpr static uint32_t MATRIX_SIZE = M::MATRIX_SIZE;
    /// Index of the first vector entry
    constexpr static uint32_t MATRIX_FIRST_INDEX = 0;
    /// Index of the last vector entry
    constexpr static uint32_t MATRIX_LAST_INDEX = MATRIX_SIZE * MATRIX_SIZE - 1;

    /// Type alias for the float|scalar type in use
    using T = typename M::ElementType;
    /// Type alias for this comma-initializer
    using Type = MatCommaInitializer<M>;
    /// Type alias for the vector type linked to this initializer
    using MatrixType = M;

    /// Constructs a comma-initializer for the given vector and initial coeff.
    // NOLINTNEXTLINE(runtime/references)
    explicit MatCommaInitializer(MatrixType& vec, const T& coeff0)
        : m_MatrixRef(vec) {
        // Append first entry of the matrix
        m_MatrixRef[0][0] = coeff0;
        ++m_CurrentBuildIndex;
    }

    // Follow RAII and the 'Rule of 5' (use defaults though) -------------------

    MatCommaInitializer(const MatCommaInitializer<M>& other) = default;

    auto operator=(const MatCommaInitializer<M>& rhs)
        -> MatCommaInitializer<M>& = default;

    MatCommaInitializer(MatCommaInitializer<M>&& other) noexcept = default;

    auto operator=(MatCommaInitializer<M>&& rhs) noexcept
        -> MatCommaInitializer<M>& = default;

    /// Release any resources and terminate the initializer's operation
    ~MatCommaInitializer() { _finished(); }

    // -------------------------------------------------------------------------

    /// Appends the given coefficient to the initializer for building the vec3
    auto operator,(const T& next_coeff) -> Type& {
        assert(m_CurrentBuildIndex <= MATRIX_LAST_INDEX);
        // Get the indices of the current (col, row) we're dealing with (notice
        // that the current index grows in row-major order, unlike our layout)
        const uint32_t row_index = m_CurrentBuildIndex / MATRIX_SIZE;  // NOLINT
        const uint32_t col_index = m_CurrentBuildIndex % MATRIX_SIZE;  // NOLINT
        m_MatrixRef[col_index][row_index] = next_coeff;
        ++m_CurrentBuildIndex;
        return *this;
    }

 private:
    /// Terminates the operations of the initializer and returns the built vec3
    MATH3D_INLINE auto _finished() -> void {
        assert(m_CurrentBuildIndex == (MATRIX_LAST_INDEX + 1));
    }

 private:
    /// Mutable reference to the matrix we're currently constructing
    MatrixType& m_MatrixRef;  // NOLINT
    /// Index of the current coefficient being built
    uint32_t m_CurrentBuildIndex = MATRIX_FIRST_INDEX;
};

/// Returns the value clamped to the given range
template <typename T>
auto clamp(T x, T x_min, T x_max) -> T {
    return std::max(x_min, std::min(x_max, x));
}

}  // namespace math
