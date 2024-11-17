#include <iostream>
#include <fstream>
#include <rbd_bool.h>
#include <rbd.h>
#include <rbd_utility.h>
using namespace rbd_bool;
using namespace rbd_utility;

namespace unit_test {
    std::map<int, std::string> int_alphabet_map = create_int_alphabet_map();
    std::map<std::string, int> alphabet_int_map = create_alphabet_int_map();

    void test_bridge_RBD() {
        std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/bridge_rbd.json");
        ProbabilityArray prob_array = read_probability_array("../topologies/bridge_rbd.json");
        for (const auto &min_cutset : min_cutsets)
        {
            // std::cout << "Current handling minimal cutsets from Src: " << min_cutset.src_dst.first << " to Dst: " << min_cutset.src_dst.second << std::endl;
            // std::cout << "Min-cutsets: " << std::endl;
            // for (const auto &cutset : min_cutset.min_cutsets)
            // {
            //     print_vector_int(cutset);
            // }
            // 
            // std::cout << "Probability sets: " << std::endl;
            // for (const auto &set : prob_set)
            // {
            //     print_vector_int(set);
            // }
            std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
            double unavailablity_from_rbd_bool = compute_probability(prob_set, prob_array);
            // std::cout << "rbd_bool computed unavailability: " << unavailablity_from_rbd_bool << std::endl;
            std::vector<std::vector<int>> prob_set_from_pyrbd ={{1,-2,3,-4},{1,-2,-3,4,-5},{1,-2,-3,-4},{-1,2,3,-4,-5},{-1,-2,3,-4},{-1,-3}};
            double unavailablity_from_pyrbd = compute_probability(prob_set_from_pyrbd, prob_array);
            // std::cout << "pyrbd computed unavailability: " << unavailablity_from_pyrbd << std::endl;
            assert(std::abs(unavailablity_from_rbd_bool - unavailablity_from_pyrbd) < 1e-9);
            std::cout << "Test bridge RBD pass! "<< std::endl;
        }
    }

    void test_example_RBD_01() {
        std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/example_rbd_01.json");
        ProbabilityArray prob_array = read_probability_array("../topologies/example_rbd_01.json");
        for (const auto &min_cutset : min_cutsets)
        {
            std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
            std::vector<std::vector<int>> prob_set_from_paper = {{1, 3}, {1, -3, 4}, {-1, 2, 3}, {-1, 2, -3, 4}};
            double unavailablity_from_rbd_bool = compute_probability(prob_set, prob_array);
            double availablity_from_paper = compute_probability(prob_set_from_paper, prob_array);
            assert(unavailablity_from_rbd_bool + availablity_from_paper == 1);
            std::cout << "Test example RBD 01 from paper pass! " << std::endl;
        }    
    }

    void test_example_RBD_02() {
        std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/example_rbd_02.json");
        ProbabilityArray prob_array = read_probability_array("../topologies/example_rbd_02.json");
        for (const auto &min_cutset : min_cutsets)
        {
            std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
            std::vector<std::vector<int>> prob_set_from_paper = {{1}, {-1, 2}};
            double unavailablity_from_rbd_bool = compute_probability(prob_set, prob_array);
            double availablity_from_paper = compute_probability(prob_set_from_paper, prob_array);
            assert(unavailablity_from_rbd_bool + availablity_from_paper == 1);
            std::cout << "Test example RBD 02 from paper pass! " << std::endl;
        }    
    }

    void test_example_RBD_03() {
        std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/example_rbd_03.json");
        ProbabilityArray prob_array = read_probability_array("../topologies/example_rbd_03.json");
        for (const auto &min_cutset : min_cutsets)
        {
            std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
            std::vector<std::vector<int>> prob_set_from_paper = {{2, 4}, {1, -2, 4}, {1, -4, 5}, {-1, -2, 3, 5}, {-1, 2, 3, -4, 5}};
            double unavailablity_from_rbd_bool = compute_probability(prob_set, prob_array);
            double availablity_from_paper = compute_probability(prob_set_from_paper, prob_array);
            assert(unavailablity_from_rbd_bool + availablity_from_paper == 1);
            std::cout << "Test example RBD 03 from paper pass! " << std::endl;
        }    
    }

  
}

int main() {
    using namespace unit_test;
    test_bridge_RBD();
    test_example_RBD_01();
    test_example_RBD_02();
    test_example_RBD_03();
    return 0;
}