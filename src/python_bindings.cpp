#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <rbd.h>

namespace py = pybind11;
using namespace rbd;

PYBIND11_MODULE(rbd_bindings, m) {
//     m.def("evaluateAvailability_old", &rbd::evaluateAvailability_old, 
//           "Evaluate availability for a specific src-dst pair and topology file.",
//           py::arg("file_name"), py::arg("src"), py::arg("dst"));
          
    m.def("evaluateAvailability", 
          [](const std::vector<std::vector<int>>& min_cutsets, 
             const std::map<int,double>& probabilities, 
             int src, int dst) {
              ProbabilityArray prob_array(probabilities);
              return rbd::evaluateAvailability(min_cutsets, prob_array, src, dst);
          },
          "Directly evaluate availability using minimal cutsets and probabilities",
          py::arg("min_cutsets"), py::arg("probabilities"), py::arg("src"), py::arg("dst"));

    m.def("lengthOfProbaset", &rbd::lengthOfProbaset,
            "Compute the length of the probability set",
            py::arg("min_cutsets"), py::arg("src"), py::arg("dst"));
}
