#include "rbd_bool.h"

using json = nlohmann::json;
// TODO: For debug
using namespace rbd_utility;
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

        // RC set: the elements in set1 but not in set2
        std::vector<int> RC;

        for (const int &elem : set1)
        {
            if (std::find(set2.begin(), set2.end(), elem) == set2.end())
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
            else if (set2_copy.back() == INT32_MIN)
            {
                set2_copy.back() = 0;
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

    // std::vector<std::vector<int>> convert_pathset_to_probaset(const std::vector<std::vector<int>> &path_sets)
    // {
    //     std::vector<std::vector<int>> prob_sets;
    //     std::vector<std::vector<int>> temp_sets{path_sets.begin(), path_sets.end()};
    //     // int cur_index = 0;

    //     while (temp_sets.size() > 0)
    //     {
    //         if (temp_sets.size() == 1)
    //         {
    //             prob_sets.push_back(temp_sets[0]);
    //             break;
    //         }
    //         std::vector<int> selected_set = temp_sets.front();
    //         prob_sets.push_back(selected_set);
    //         std::vector<std::vector<int>> remaining_sets(temp_sets.begin() + 1, temp_sets.end());
    //         temp_sets.clear();

    //         for (const auto &set : remaining_sets)
    //         {
    //             std::vector<std::vector<int>> disjoint_sets = create_disjoint_set(selected_set, set);
    //             for (const auto &disjoint_set : disjoint_sets)
    //             {
    //                 temp_sets.push_back(disjoint_set);
    //             }
    //         }
    //     }
    //     return prob_sets;
    // }

    std::vector<std::vector<int>> convert_mincutset_to_probaset(const std::vector<std::vector<int>> &min_cutsets, const std::pair<int, int> &src_dst)
    {
        std::vector<std::vector<int>> prob_sets;
        std::vector<std::vector<int>> temp_sets{min_cutsets.begin(), min_cutsets.end()};

        // remove the sets: {src} and {dst}
        auto it_src = std::find(temp_sets.begin(), temp_sets.end(), std::vector<int>{src_dst.first});
        if (it_src != temp_sets.end())
        {
            temp_sets.erase(it_src);
        }
        auto it_dst = std::find(temp_sets.begin(), temp_sets.end(), std::vector<int>{src_dst.second});
        if (it_dst != temp_sets.end())
        {
            temp_sets.erase(it_dst);
        }
        if (temp_sets.empty())
        {
            // Debug print
            // std::cout << "The probaset are empty!" << std::endl;

            return {};
        }

        // inverse the min_cutsets
        for (auto &set : temp_sets)
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

        // convert the min_cutsets to the probability sets
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

    // double compute_avail(const std::pair<int, int> &src_dst, const std::vector<std::vector<int>> &prob_set, const ProbabilityArray &prob_array)
    // {

    //     double result = 0;
    //     for (const auto &set : prob_set)
    //     {
    //         double temp = 1;
    //         for (const auto &num : set)
    //         {
    //             // -0 is represented by INT32_MIN
    //             // ProbabilityArray class will handle this case
    //             temp *= prob_array[num];
    //         }
    //         result += temp;
    //     }
    //     // return prob_array[src_dst.first] * prob_array[src_dst.second] * result;
    //     return result;
    // }

    double compute_avail(const std::pair<int, int> &src_dst, std::vector<std::vector<int>> &prob_set, const ProbabilityArray &prob_array)
    {
        // Debug print
        std::cout << "The unavailability are caculating with form: " << std::endl;

        // Save the final result
        double unavil = 0.0;
        for (const auto &set : prob_set)
        {
            // Save the temp result
            double temp = 1.0;
            for (const auto &num : set)
            {
                // Debug print
                if (num == INT32_MIN)
                {
                    std::cout << "-0" << " * ";
                }
                else
                {
                    std::cout << num << " * ";
                }
                temp *= prob_array[num];
            }
            // Debug print
            std::cout << " += " << temp << std::endl;

            unavil += temp;
        }

        double avail = 1.0 - unavil;
        double avail_total = prob_array[src_dst.first] * prob_array[src_dst.second] * avail;

        // Debug print
        std::cout << "The unavailability between ";
        std::cout << "1.0 - unavailability = " << avail << std::endl;
        std::cout << "The final availability is " << prob_array[src_dst.first] << " * " << prob_array[src_dst.second] << " * " << avail 
        << " = " << avail_total << std::endl;
        return avail_total;
    }

    std::map<std::pair<int, int>, double> evaluate_avail(const std::string file_name)
    {

        // read the minimal cut set and the probability array
        std::vector<MinCutset> min_cutsets = read_minimal_cut_set(file_name);
        ProbabilityArray prob_array = read_probability_array(file_name);

        // save the result
        std::map<std::pair<int, int>, double> result;

        // evaluate the unavailability for each src-dst pair
        for (const auto &cutset : min_cutsets)
        {
            // Debug print
            std::cout << "=============================================" << std::endl;
            std::cout << "Handling the min-cutsets from " << cutset.src_dst.first << " to " << cutset.src_dst.second << ":" << std::endl;
            print_vector_of_vector_int(cutset.min_cutsets);

            std::vector<std::vector<int>> prob_sets = convert_mincutset_to_probaset(cutset.min_cutsets, cutset.src_dst);
            // Debug print
            std::cout << "The probability sets are: " << std::endl;
            print_vector_of_vector_int(prob_sets);

            double prob = compute_avail(cutset.src_dst, prob_sets, prob_array);

            // Debug print
            std::cout << "=============================================" << std::endl;            
            result[cutset.src_dst] = prob;
        }
        return result;
    }
}
