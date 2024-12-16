#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <rbd.h>

namespace py = pybind11;

PYBIND11_MODULE(rbd_bindings, m) {
    m.def("evaluateAvailability", &rbd::evaluateAvailability, 
          "Evaluate availability for a specific src-dst pair and topology file.",
          py::arg("file_name"), py::arg("src"), py::arg("dst"));
}