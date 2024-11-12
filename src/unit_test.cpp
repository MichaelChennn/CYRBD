#include <iostream>
#include <fstream>
#include <rbd_bool.h>
#include <rbd.h>
#include <rbd_utility.h>
using namespace rbd_bool;
using namespace rbd_utility;

namespace unit_test {
    void test_bridge_RBD() {
        // std::map<std::string, int> alphabet_int_map = create_alphabet_int_map();
        std::map<int, std::string> int_alphabet_map = create_int_alphabet_map();
        std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/bridge_rbd.json");
        ProbabilityArray prob_array = read_probability_array("../topologies/bridge_rbd.json");
        for (const auto &min_cutset : min_cutsets)
        {
            std::cout << "Current handling minimal cutsets from Src: " << min_cutset.src_dst.first << " to Dst: " << min_cutset.src_dst.second << std::endl;
            std::cout << "Min-cutsets: " << std::endl;
            for (const auto &cutset : min_cutset.min_cutsets)
            {
                print_vector_int(cutset);
            }
            std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
            std::cout << "Probability sets: " << std::endl;
            for (const auto &set : prob_set)
            {
                print_vector_int(set);
            }
            double unavailable_probability_from_rbd_bool = compute_probability(prob_set, prob_array);
            std::cout << "rbd_bool computed unavailability: " << unavailable_probability_from_rbd_bool << std::endl;
            std::vector<std::vector<int>> prob_set_from_pyrbd ={{1,-2,3,-4},{1,-2,-3,4,-5},{1,-2,-3,-4},{-1,2,3,-4,-5},{-1,-2,3,-4},{-1,-3}};
            double unavailable_probability_from_pyrbd = compute_probability(prob_set_from_pyrbd, prob_array);
            std::cout << "pyrbd computed unavailability: " << unavailable_probability_from_pyrbd << std::endl;
            assert(std::abs(unavailable_probability_from_rbd_bool - unavailable_probability_from_pyrbd) < 1e-9);
        }
    }
}

int main() {
    using namespace unit_test;
    test_bridge_RBD();
    return 0;
}