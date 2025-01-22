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
          [](std::vector<std::vector<int>>& mincutset, 
             const std::map<int,double>& probabilities, 
             int src, int dst) {
              ProbabilityArray probArr(probabilities);
              return rbd::evaluateAvailability(src, dst, probArr, mincutset);
          },
          "Directly evaluate availability using minimal cutsets and probabilities",
          py::arg("mincutset"), py::arg("probabilities"), py::arg("src"), py::arg("dst"), py::call_guard<py::gil_scoped_release>());

    m.def("evaluateAvailabilityTopology", 
          [](std::vector<std::vector<std::vector<int>>>& mincutsets, 
             const std::map<int,double>& probabilities, 
             const std::vector<std::pair<int,int>>& nodePairs) {
              ProbabilityArray prob_array(probabilities);
              return rbd::evaluateAvailabilityTopology(nodePairs, prob_array, mincutsets);
          },
          "Directly evaluate availability using minimal cutsets and probabilities",
          py::arg("mincutset"), py::arg("probabilities"), py::arg("nodePairs"), py::call_guard<py::gil_scoped_release>());

    m.def("boolExprCount", &rbd::boolExprCount,
            "Compute the length of the probability set",
            py::arg("mincutset"), py::arg("src"), py::arg("dst"));
}
