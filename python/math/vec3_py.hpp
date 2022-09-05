#pragma once

#include <cstring>
#include <stdexcept>
#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>
#include <pybind11/attr.h>

#include <loco/math/vec3_t_impl.hpp>

#include <common_py.hpp>

namespace py = pybind11;

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

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
            BUFFER_PROTOCOL(T)
            PROPERTY(x)
            PROPERTY(y)
            PROPERTY(z)
            OPERATORS(T)
            METHODS(T)
            GETSET_ITEM(Class::VECTOR_SIZE, T)
            // clant-format on
            // NOLINTNEXTLINE
            .def_property_readonly("ndim", [](const Class& self) {
                return Class::VECTOR_NDIM;
            })
            // NOLINTNEXTLINE
            .def_property_readonly("shape", [](const Class& self) {
                return std::pair<uint32_t, uint32_t>(1, Class::VECTOR_SIZE);
            })
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str("Vector3{}(x={}, y={}, z={})")
                    .format((IsFloat32<T>::value ? "f" : "d"), self.x(),
                            self.y(), self.z());
            });
    }
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

}  // namespace math
}  // namespace loco
