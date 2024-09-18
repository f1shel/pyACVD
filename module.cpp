#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "pyacvd/interface.h"

namespace py = pybind11;

PYBIND11_MODULE(pyacvd, m) {
    m.doc() = "pybind11 pyacvd module";
    m.def("acvdqp", &acvdqp,
        py::arg("vertices"),
        py::arg("faces"),
        py::arg("numberOfSamples") = 500,
        py::arg("gradation") = 0.0,
        py::arg("subsamplingThreshold") = 10,
        py::arg("forceManifold") = false,
        py::arg("NumberOfThreads") = 0,
        "remeshing"
    );
}