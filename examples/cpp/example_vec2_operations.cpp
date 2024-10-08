#include <iostream>

#include <math/vec2_t.hpp>

template <typename T,
          typename = typename std::enable_if<math::IsScalar<T>::value>::type>
MATH3D_NEVER_INLINE auto run_operations_vec2() -> void {
    using Vec2 = math::Vector2<T>;

    // Preamble (show the type we're currently working with)
    if (std::is_same<T, float>()) {
        std::cout << "Vector2-float32 type:\n";
    } else if (std::is_same<T, double>()) {
        std::cout << "Vector2-float64 type:\n";
    }

    Vec2 vec_a(1.0, 2.0);
    Vec2 vec_b(3.0, 5.0);

    Vec2 vec_sum = vec_a + vec_b;
    Vec2 vec_diff = vec_a - vec_b;
    Vec2 vec_scale_1 = 2.5 * vec_a;
    Vec2 vec_scale_2 = vec_b * 0.25;
    Vec2 vec_mul = vec_a * vec_b;

    std::cout << "a: " << '\n' << vec_a.toString() << '\n';
    std::cout << "b: " << '\n' << vec_b.toString() << '\n';
    std::cout << "a + b: " << '\n' << vec_sum.toString() << '\n';
    std::cout << "a - b: " << '\n' << vec_diff.toString() << '\n';
    std::cout << "2.5 * a: " << '\n' << vec_scale_1.toString() << '\n';
    std::cout << "b * 0.25: " << '\n' << vec_scale_2.toString() << '\n';
    std::cout << "a . b: " << '\n' << math::dot(vec_a, vec_b) << '\n';
    std::cout << "b . a: " << '\n' << math::dot(vec_b, vec_a) << '\n';
    std::cout << "|a|: " << '\n' << math::norm(vec_a) << '\n';
    std::cout << "|a|^2: " << '\n' << math::squareNorm(vec_a) << '\n';
    std::cout << "|b|: " << '\n' << math::norm(vec_b) << '\n';
    std::cout << "|b|^2: " << '\n' << math::squareNorm(vec_b) << '\n';
    std::cout << "a * b: " << '\n' << vec_mul.toString() << '\n';
    std::cout << "a == b: " << '\n'
              << ((vec_a == vec_b) ? "True" : "False") << '\n';
    std::cout << "a != b: " << '\n'
              << ((vec_a != vec_b) ? "True" : "False") << '\n';
}

auto main() -> int {
    run_operations_vec2<float>();
    run_operations_vec2<double>();
    return 0;
}
