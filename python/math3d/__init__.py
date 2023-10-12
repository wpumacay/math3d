# Helper functions used for numpy convertions
# fmt: off
from math3d_bindings import (
    AABB_d,
    AABB_f,
    Euler_d,
    Euler_f,
    Line_d,
    Line_f,
    Matrix2d,
    Matrix2f,
    Matrix3d,
    Matrix3f,
    Matrix4d,
    Matrix4f,
    Plane_d,
    Plane_f,
    Pose3d_d,
    Pose3d_f,
    Quaterniond,
    Quaternionf,
    Vector2d,
    Vector2f,
    Vector3d,
    Vector3f,
    Vector4d,
    Vector4f,
    eConvention,
    eOrder,
    mat2_to_nparray_f32,
    mat2_to_nparray_f64,
    mat3_to_nparray_f32,
    mat3_to_nparray_f64,
    mat4_to_nparray_f32,
    mat4_to_nparray_f64,
    nparray_to_mat2_f32,
    nparray_to_mat2_f64,
    nparray_to_mat3_f32,
    nparray_to_mat3_f64,
    nparray_to_mat4_f32,
    nparray_to_mat4_f64,
    nparray_to_vec2_f32,
    nparray_to_vec2_f64,
    nparray_to_vec3_f32,
    nparray_to_vec3_f64,
    nparray_to_vec4_f32,
    nparray_to_vec4_f64,
    vec2_to_nparray_f32,
    vec2_to_nparray_f64,
    vec3_to_nparray_f32,
    vec3_to_nparray_f64,
    vec4_to_nparray_f32,
    vec4_to_nparray_f64,
)

# fmt: on

__all__ = [
    # math3d types
    "Vector2f",
    "Vector2d",
    "Vector3f",
    "Vector3d",
    "Vector4f",
    "Vector4d",
    "Matrix2f",
    "Matrix2d",
    "Matrix3f",
    "Matrix3d",
    "Matrix4f",
    "Matrix4d",
    "Quaternionf",
    "Quaterniond",
    "Euler_f",
    "Euler_d",
    "eOrder",
    "eConvention",
    "Pose3d_f",
    "Pose3d_d",
    # utilities
    "Line_f",
    "Line_d",
    "Plane_f",
    "Plane_d",
    "AABB_f",
    "AABB_d",
    # math3d -> numpy conversions
    "vec2_to_nparray_f32",
    "vec2_to_nparray_f64",
    "vec3_to_nparray_f32",
    "vec3_to_nparray_f64",
    "vec4_to_nparray_f32",
    "vec4_to_nparray_f64",
    "mat2_to_nparray_f32",
    "mat2_to_nparray_f64",
    "mat3_to_nparray_f32",
    "mat3_to_nparray_f64",
    "mat4_to_nparray_f32",
    "mat4_to_nparray_f64",
    # numpy -> math3d conversions
    "nparray_to_vec2_f32",
    "nparray_to_vec2_f64",
    "nparray_to_vec3_f32",
    "nparray_to_vec3_f64",
    "nparray_to_vec4_f32",
    "nparray_to_vec4_f64",
    "nparray_to_mat2_f32",
    "nparray_to_mat2_f64",
    "nparray_to_mat3_f32",
    "nparray_to_mat3_f64",
    "nparray_to_mat4_f32",
    "nparray_to_mat4_f64",
]
