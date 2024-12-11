#include <iostream>
#include <fstream>
#include <rbd_bool.h>
#include <rbd_utility.h>
using namespace rbd_bool;
using namespace rbd_utility;

namespace unit_test {
    std::map<int, std::string> int_alphabet_map = create_int_alphabet_map();
    std::map<std::string, int> alphabet_int_map = create_alphabet_int_map();

    void test_bridge_RBD() {
        std::cout << "==========Test Bridge RBD==========" << std::endl;
        std::string file_path = "../topologies/bridge_rbd.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluate_avail(file_path);
        std::cout << std::fixed << std::setprecision(16);
        for (const auto &elem : unavail_map)
        {
            std::pair<int, int> src_dst = elem.first;
            double unavail = elem.second;
            std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        }

    }

    void test_Abilene() {
        std::cout << "==========Test Abilene==========" << std::endl;
        std::string file_path = "../topologies/Abilene.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluate_avail(file_path);
        std::cout << std::fixed << std::setprecision(16);
        for (const auto &elem : unavail_map)
        {
            std::pair<int, int> src_dst = elem.first;
            double unavail = elem.second;
            std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        }
    }

    // void test_example_RBD_01() {
        
    //     std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/example_rbd_01.json");
    //     ProbabilityArray prob_array = read_probability_array("../topologies/example_rbd_01.json");
    //     for (const auto &min_cutset : min_cutsets)
    //     {
    //         std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
    //         std::vector<std::vector<int>> prob_set_from_paper = {{1, 3}, {1, -3, 4}, {-1, 2, 3}, {-1, 2, -3, 4}};
    //         std::pair<int, int> src_dst = min_cutset.src_dst;
    //         double unavailablity_from_rbd_bool = compute_probability(src_dst, prob_set, prob_array);
    //         double availablity_from_paper = compute_probability(src_dst, prob_set_from_paper, prob_array);
    //         assert(unavailablity_from_rbd_bool + availablity_from_paper == 1);
    //         std::cout << "Test example RBD 01 from paper pass! " << std::endl;
    //     }    
    // }

    // void test_example_RBD_02() {
    //     std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/example_rbd_02.json");
    //     ProbabilityArray prob_array = read_probability_array("../topologies/example_rbd_02.json");
    //     for (const auto &min_cutset : min_cutsets)
    //     {
    //         std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
    //         std::vector<std::vector<int>> prob_set_from_paper = {{1}, {-1, 2}};
    //         std::pair<int, int> src_dst = min_cutset.src_dst;
    //         double unavailablity_from_rbd_bool = compute_probability(src_dst, prob_set, prob_array);
    //         double availablity_from_paper = compute_probability(src_dst, prob_set_from_paper, prob_array);
    //         assert(unavailablity_from_rbd_bool + availablity_from_paper == 1);
    //         std::cout << "Test example RBD 02 from paper pass! " << std::endl;
    //     }    
    // }

    // void test_example_RBD_03() {
    //     std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/example_rbd_03.json");
    //     ProbabilityArray prob_array = read_probability_array("../topologies/example_rbd_03.json");
    //     for (const auto &min_cutset : min_cutsets)
    //     {
    //         std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
    //         std::vector<std::vector<int>> prob_set_from_paper = {{2, 4}, {1, -2, 4}, {1, -4, 5}, {-1, -2, 3, 5}, {-1, 2, 3, -4, 5}};
    //         std::pair<int, int> src_dst = min_cutset.src_dst;
    //         double unavailablity_from_rbd_bool = compute_probability(src_dst, prob_set, prob_array);
    //         double availablity_from_paper = compute_probability(src_dst, prob_set_from_paper, prob_array);
    //         assert(unavailablity_from_rbd_bool + availablity_from_paper == 1);
    //         std::cout << "Test example RBD 03 from paper pass! " << std::endl;
    //     }    
    // }

    void fake_test() {
        double result = 0.99 * 0.99 * 0.999898;
        std::cout << result << std::endl;
    }

  
}

int main() {
    using namespace unit_test;
    // test_bridge_RBD();
    test_Abilene();
    // fake_test();
    // test_example_RBD_01();
    // test_example_RBD_02();
    // test_example_RBD_03();

    return 0;
}