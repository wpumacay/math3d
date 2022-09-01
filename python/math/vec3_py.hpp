#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>
#include <pybind11/attr.h>

#include <loco/math/vec3_t_impl.hpp>

#include <common_py.hpp>

namespace py = pybind11;

namespace loco {
namespace math {

template <typename T>
using SFINAE_VEC3_BINDINGS = typename std::enable_if<IsScalar<T>::value>::type*;

template <typename T, SFINAE_VEC3_BINDINGS<T> = nullptr>
// NOLINTNEXTLINE
auto bindings_vector3(py::module& m, const char* class_name) -> void {
    {
        using Class = Vector3<T>;
        py::class_<Class>(m, class_name, py::buffer_protocol())
            .def(py::init<>())
            .def(py::init<T>())
            .def(py::init<T, T>())
            .def(py::init<T, T, T>())
            // clang-format off
            PROPERTY(x)
            PROPERTY(y)
            PROPERTY(z)
            OPERATORS(T)
            METHODS(T)
            GETSET_ITEM(Class::VECTOR_NDIM, T)
            // clant-format on
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str("Vector3{}(x={}, y={}, z={})")
                    .format((IsFloat32<T>::value ? "f" : "d"), self.x(),
                            self.y(), self.z());
            });
    }
}

}  // namespace math
}  // namespace loco
