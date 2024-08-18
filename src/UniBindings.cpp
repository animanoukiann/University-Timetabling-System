#include "pybind11/pybind11.h"
#include "University.hpp"
namespace py = pybind11;

PYBIND11_MODULE(pyuniversity, m) {
    py::class_<University>(m, "University")
        .def(py::init<>())
        .def("schedule", &University::schedule);
}
