#include <rbd.h>
using namespace rbd;


namespace unit_test {
    void test_bridge_RBD() {
        std::cout << "==========Test Bridge RBD==========" << std::endl;
        std::string file_path = "../topologies/bridge_rbd.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluateAvailabilityTopology(file_path);
        std::cout << std::fixed << std::setprecision(9);
        for (const auto &elem : unavail_map)
        {
            std::pair<int, int> src_dst = elem.first;
            double unavail = elem.second;
            std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        }

    }

    void test_Abilene() {
        std::cout << "==========Test Abilene==========" << std::endl;
        std::string file_path = "../topologies/Abilene/Abilene.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluateAvailabilityTopology(file_path);
        std::cout << std::fixed << std::setprecision(9);
        std::string topology_name = "Abilene";
        writeResultToFile(topology_name, unavail_map);
        // for (const auto &elem : unavail_map)
        // {
        //     std::pair<int, int> src_dst = elem.first;
        //     double unavail = elem.second;
        //     std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        // }
    }

    void test_Germany_17() {
        std::cout << "==========Test Germany_17==========" << std::endl;
        std::string file_path = "../topologies/Germany_17/Germany_17.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluateAvailabilityTopology(file_path);
        std::cout << std::fixed << std::setprecision(9);
        std::string topology_name = "Germany_17";
        writeResultToFile(topology_name, unavail_map);
        // for (const auto &elem : unavail_map)
        // {
        //     std::pair<int, int> src_dst = elem.first;
        //     double unavail = elem.second;
        //     std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        // }
    }

    void test_HibernialUk() {
        std::cout << "==========Test Hibernia UK==========" << std::endl;
        std::string file_path = "../topologies/HiberniaUk/HiberniaUk.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluateAvailabilityTopology(file_path);
        std::cout << std::fixed << std::setprecision(9);
        std::string topology_name = "HiberniaUk";
        writeResultToFile(topology_name, unavail_map);
        // for (const auto &elem : unavail_map)
        // {
        //     std::pair<int, int> src_dst = elem.first;
        //     double unavail = elem.second;
        //     std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        // }
    }

    void test_dfn_bwin() {
        std::cout << "==========Test DFN BWIN==========" << std::endl;
        std::string file_path = "../topologies/dfn-bwin/dfn-bwin.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluateAvailabilityTopology(file_path);
        std::cout << std::fixed << std::setprecision(9);
        std::string topology_name = "dfn-bwin";
        writeResultToFile(topology_name, unavail_map);
        // for (const auto &elem : unavail_map)
        // {
        //     std::pair<int, int> src_dst = elem.first;
        //     double unavail = elem.second;
        //     std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        // }
    }

    void test_polska () {
        std::cout << "==========Test Polska==========" << std::endl;
        std::string file_path = "../topologies/polska/polska.json";
        std::map<std::pair<int, int>, double> unavail_map = evaluateAvailabilityTopology(file_path);
        std::cout << std::fixed << std::setprecision(9);
        std::string topology_name = "polska";
        writeResultToFile(topology_name, unavail_map);
        // for (const auto &elem : unavail_map)
        // {
        //     std::pair<int, int> src_dst = elem.first;
        //     double unavail = elem.second;
        //     std::cout << "The availability from " << src_dst.first << " to " << src_dst.second << " is " << unavail << std::endl;
        // }
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

    // void fake_test() {
    //     std::unordered_map<std::pair<int, int>, std::vector<std::vector<int>>> min_cutsets;
    //     std::vector<std::vector<int>> min_cutset_1 = {{1, 3}, {1, -3, 4}, {-1, 2, 3}, {-1, 2, -3, 4}};
    //     std::vector<std::vector<int>> min_cutset_2 = {{1}, {-1, 2}};
    //     std::vector<std::vector<int>> min_cutset_3 = {{2, 4}, {1, -2, 4}, {1, -4, 5}, {-1, -2, 3, 5}, {-1, 2, 3, -4, 5}};
    //     std::pair<int, int> src_dst_1 = {1, 4};
    //     std::pair<int, int> src_dst_2 = {1, 2};
    //     std::pair<int, int> src_dst_3 = {2, 5};
    //     min_cutsets[src_dst_1] = min_cutset_1;
    //     min_cutsets[src_dst_2] = min_cutset_2;
    //     min_cutsets[src_dst_3] = min_cutset_3;

    //     std::pair<int, int> src_dst_4 = {1, 4};
    //     print_vector_of_vector_int(min_cutsets[src_dst_4]);
    // }

  
}

int main() {
    using namespace unit_test;
    // test_bridge_RBD();
    test_Abilene();
    test_Germany_17();
    test_HibernialUk();
    test_dfn_bwin();
    test_polska();
    // fake_test();
    // test_example_RBD_01();
    // test_example_RBD_02();
    // test_example_RBD_03();

    return 0;
}