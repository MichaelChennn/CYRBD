#include "rbd_bool.h"

using json = nlohmann::json;

namespace rbd_bool
{

    std::vector<MinCutset> read_minimal_cut_set(const std::string file_path)
    {
        // open the json file
        std::ifstream
            file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Cannot open minimal cut set file" << std::endl;
            exit(1);
        }

        // create a vector of MinCutset
        std::vector<MinCutset> min_cutsets;

        // read the json file
        json j = json::parse(file);
        if (j.contains("minimal_cutsets"))
        {
            for (const auto &j_element : j["minimal_cutsets"])
            {
                MinCutset cutset;
                cutset.src_dst = {j_element["src-dst"][0], j_element["src-dst"][1]};
                for (const auto &j_cut_set : j_element["min-cutsets"])
                {
                    std::vector<int> cutsetVec(j_cut_set.begin(), j_cut_set.end());
                    std::sort(cutsetVec.begin(), cutsetVec.end());
                    cutset.min_cutsets.push_back(cutsetVec);
                }
                min_cutsets.push_back(cutset);
            }
        }
        else
        {
            std::cerr << "Cannot find minimal_cutsets in the json file" << std::endl;
        }

        // close the file
        file.close();

        return min_cutsets;
    }

    ProbabilityArray read_probability_array(const std::string file_path)
    {
        // open the json file
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Cannot open probabilty file" << std::endl;
            exit(1);
        }

        // read the json file
        json j = json::parse(file);
        std::vector<double> pos_array = j["probability"].get<std::vector<double>>();

        // create the ProbabilityArray object
        ProbabilityArray prob_array(pos_array);

        // close the file
        file.close();

        return prob_array;
    }

    std::vector<std::vector<int>> create_disjoint_set(std::vector<int> set1, std::vector<int> set2)
    {
        // check if the set1 and set2 are already disjoint
        // with the rule：when x in set1 and x' in set2, set1 and set2 are disjoint
        // take care of the sepcial case: -0 is represented by INT32_MIN
        for (const int &elem : set1)
        {
            if (elem == 0)
            {
                if (std::find(set2.begin(), set2.end(), INT32_MIN) != set2.end())
                {
                    return {set2};
                }
            }
            else if (elem == INT32_MIN)
            {
                if (std::find(set2.begin(), set2.end(), 0) != set2.end())
                {
                    return {set2};
                }
            }
            else if (std::find(set2.begin(), set2.end(), -elem) != set2.end())
            {
                return {set2};
            }
        }

        std::vector<std::vector<int>> result;

        // find RC set
        std::unordered_set<int> set2_set(set2.begin(), set2.end());
        std::vector<int> RC;

        for (const int &elem : set1)
        {
            if (set2_set.find(elem) == set2_set.end())
            {
                RC.push_back(elem);
            }
        }

        // TODO: Absorption rule: x + xy = x, check if the RC is empty

        if (RC.empty())
        {
            return std::vector<std::vector<int>>{};
        }


        // create disjoint sets
        for (size_t i = 0; i < RC.size(); i++)
        {

            // every time add one more element from RC to set2 and the last element to add is negative.
            // special case: -0 is represented by INT32_MIN
            set2.push_back(RC[i]);
            // copy the set2 to make sure the original set2 is not changed
            std::vector<int> set2_copy = set2;
            if (set2_copy.back() == 0)
            {
                set2_copy.back() = INT32_MIN;
            }
            else
            {
                set2_copy.back() = -set2_copy.back();
            }

            // sort the set to be added
            std::sort(set2_copy.begin(), set2_copy.end());

            // save the new set
            result.push_back(set2_copy);
        }

        return result;
    }

    std::vector<std::vector<int>> convert_pathset_to_probaset(const std::vector<std::vector<int>> &path_sets)
    {
        std::vector<std::vector<int>> prob_sets;
        std::vector<std::vector<int>> temp_sets{path_sets.begin(), path_sets.end()};
        // int cur_index = 0;

        while (temp_sets.size() > 0)
        {
            if (temp_sets.size() == 1)
            {
                prob_sets.push_back(temp_sets[0]);
                break;
            }
            std::vector<int> selected_set = temp_sets.front();
            prob_sets.push_back(selected_set);
            std::vector<std::vector<int>> remaining_sets(temp_sets.begin() + 1, temp_sets.end());
            temp_sets.clear();

            for (const auto &set : remaining_sets)
            {
                std::vector<std::vector<int>> disjoint_sets = create_disjoint_set(selected_set, set);
                for (const auto &disjoint_set : disjoint_sets)
                {
                    temp_sets.push_back(disjoint_set);
                }
            }
        }
        return prob_sets;
    }

    std::vector<std::vector<int>> convert_mincutset_to_probaset(const std::vector<std::vector<int>> &min_cutsets)
    {
        std::vector<std::vector<int>> min_cutsets_copy = min_cutsets;
        // inverse the min_cutsets
        for (auto &set : min_cutsets_copy)
        {
            for (auto &elem : set)
            {
                if (elem == INT32_MIN)
                {
                    elem = 0;
                }
                else if (elem == 0)
                {
                    elem = INT32_MIN;
                }
                else
                {
                    elem = -elem;
                }
            }
        }

        return convert_pathset_to_probaset(min_cutsets_copy);
    }

    double compute_probability(const std::vector<std::vector<int>> &prob_set, const ProbabilityArray &prob_array)
    {
        double result = 0;
        for (const auto &set : prob_set)
        {
            double temp = 1;
            for (const auto &num : set)
            {
                // -0 is represented by INT32_MIN
                // ProbabilityArray class will handle this case
                temp *= prob_array[num];
            }
            result += temp;
        }
        return result;
    }

}

// int main()
// {
//     // using namespace rbd_bool;
//     // using namespace rbd_utility;
//     // std::map<std::string, int> alphabet_int_map = create_alphabet_int_map();
//     // std::map<int, std::string> int_alphabet_map = create_int_alphabet_map();
//     // std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../topologies/bridge_rbd.json");

//     // for (const auto &min_cutset : min_cutsets)
//     // {
//     //     std::vector<std::vector<int>> prob_set = convert_mincutset_to_probaset(min_cutset.min_cutsets);
//     //     for (const auto &set : prob_set)
//     //     {
//     //         for (const auto &elem : set)
//     //         {
//     //             std::cout << int_alphabet_map[elem] << " ";
//     //         }
//     //         std::cout << std::endl;
//     //     }
//     //     double my_prob = compute_probability(prob_set, read_probability_array("../topologies/bridge_rbd.json"));
//     //     std::cout << "My unavail is: " << my_prob << std::endl;
//     // }
//     // ProbabilityArray prob_array = read_probability_array("../simple_graph.json");

//     // std::vector<std::vector<int>> prob_set = {{1, 3}, {1, -3, 4}, {-1, 2, 3}, {-1, 2, -3, 4}};
//     // double prob = compute_probability(prob_set, prob_array);
//     // std::cout << "The avail is: " << prob << std::endl;

//     // std::vector<std::vector<int>> fake_set = {{-1, -2}, {1, -3, -4}, {-1, 2, -3, -4}};
//     // double my_prob = compute_probability(fake_set, prob_array);
//     // std::cout << "My unavail is: " << my_prob << std::endl;
//     // double sum = my_prob + prob;
//     // std::cout << "The sum is: " << sum << std::endl;

//     // std::vector<std::vector<int>> avail_from_paper = {{1, 3, -2, -4}, {1, -3, -2, 4, -5}, {1, -3, -2, -4}, {-1, 3, 2, -4, -5}, {-1, 3, -2, -4}, {-1, -3}};
//     // double avail_prob = compute_probability(avail_from_paper, prob_array);
//     // std::cout << "The availability is: " << avail_prob << std::endl;
//     // std::vector<std::vector<int>> unavail_from_paper = {{1, 2, 3}, {1, 3, -2, 4}, {1, -3, 2}, {1, -3, -2, 4, 5}, {-1, 3, 2, 4}, {-1, 3, 2, -4, 5}, {-1, 3, -2, 4}};
//     // double unavail_prob = compute_probability(unavail_from_paper, prob_array);
//     // std::cout << "The unavailability is: " << unavail_prob << std::endl;
//     // double sum = avail_prob + unavail_prob;
//     // std::cout << "The sum is: " << sum << std::endl;
//     // std::vector<std::vector<int>> prob_set = {{1, 3}, {-1, 2, 4}, {1, 2, -3, 4}, {-1, -2, 5}, {2, -4, 1, 5},{-2,1,-3,5}};
//     // double prob = compute_probability(prob_set, prob_array);
//     // std::cout << "My compute unavailability is: " << prob << std::endl;
// }