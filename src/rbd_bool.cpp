#include "rbd_bool.h"

using json = nlohmann::json;
//TODO: For debug
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

    double compute_avail(const std::pair<int, int> &src_dst, const std::vector<std::vector<int>> &prob_set, const ProbabilityArray &prob_array)
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
        // return prob_array[src_dst.first] * prob_array[src_dst.second] * result;
        return prob_array[src_dst.first] * prob_array[src_dst.second] * result;
    }

    double compute_unavail(const std::pair<int, int> &src_dst, const std::vector<std::vector<int>> &prob_set, const ProbabilityArray &prob_array)
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

        std::pair<int, int> reversed_src_dst = reverse_src_dst(src_dst);

        return prob_array[reversed_src_dst.first] * prob_array[reversed_src_dst.second] * result;
    }

    std::map<std::pair<int, int>, double> evaluate_unavail(const std::string file_name) {
        // TODO: for debug
        std::map<int, std::string> int_alphabet_map = create_int_alphabet_map();
        std::map<std::string, int> alphabet_int_map = create_alphabet_int_map();


        std::vector<MinCutset> min_cutsets = read_minimal_cut_set(file_name);
        ProbabilityArray prob_array = read_probability_array(file_name);
        std::map<std::pair<int, int>, double> result;
        for (const auto &cutset : min_cutsets)
        {   
            //TODO: for debug
            for (const auto &set : cutset.min_cutsets)
            {
                print_vector_int(set);
            }
            std::vector<std::vector<int>> prob_sets = convert_mincutset_to_probaset(cutset.min_cutsets);
            std::cout << "src_dst: " << cutset.src_dst.first << " " << cutset.src_dst.second << std::endl;
            // TODO: for debug
            for (const auto &set : prob_sets)
            {
                print_vector_int(set);
            }
            
            double prob = compute_unavail(cutset.src_dst, prob_sets, prob_array);
            result[cutset.src_dst] = prob;
        }
        return result;
    }
}