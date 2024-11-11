#include "rbd_bool.h"


using json = nlohmann::json;

namespace rbd_bool{

    std::vector<MinCutset> read_minimal_cut_set(std::string file_path)

    {
        // open the json file
        std::ifstream
        file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Cannot open file" << std::endl;
            exit(1);
        }

        // create a vector of MinCutset
        std::vector<MinCutset> min_cutsets;
        
        // read the json file
        json j = json::parse(file);
        for (const auto &j_element : j)
        {
            MinCutset cutset;
            cutset.src_dst = {j_element["src-dst"][0], j_element["src-dst"][1]};
            for (const auto & j_cut_set: j_element["min-cutsets"])
            {
                std::vector<int> cutsetVec(j_cut_set.begin(), j_cut_set.end());
                std::sort(cutsetVec.begin(), cutsetVec.end());
                cutset.min_cutsets.push_back(cutsetVec);
            }
            min_cutsets.push_back(cutset);
        }

        // close the file
        file.close();

        return min_cutsets;
    }

    ProbabilityArray read_probability_array(std::string file_path)
    {
        // open the json file
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Cannot open file" << std::endl;
            exit(1);
        }

        // read the json file
        json j = json::parse(file);
        std::vector<double> pos_array = j["pos_array"].get<std::vector<double>>();

        // create the ProbabilityArray object
        ProbabilityArray prob_array(pos_array);

        // close the file
        file.close();

        return prob_array;
    }

    std::vector<std::vector<int>> create_disjoint_set(std::vector<int> set1, std::vector<int> set2) {
        
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

        // if the RC set is equal to set1 means set1 and set2 are disjoint
        // then return set2
        if (std::equal(set1.begin(), set1.end(), RC.begin(), RC.end()))
        {
            result.push_back(set2);
            return result;
        }

        // create disjoint sets
        for (size_t i = 0; i < RC.size(); i++)
        {   

            // every time add one more element from RC to set2 and the last element to add is negative.
            // special case: -0 is represented by INT32_MIN
            set2.push_back(RC[i]);
            // copy the set2 to make sure the original set2 is not changed
            std::vector<int> set2_copy = set2;
            if (set2_copy.back() == 0) {
                set2_copy.back() = INT32_MIN;
            } else {
                set2_copy.back() = -set2_copy.back();
            }

            // sort the set to be added
            std::sort(set2_copy.begin(), set2_copy.end());
        
            // save the new set
            result.push_back(set2_copy);
        }

        return result;
    }

    std::vector<std::vector<int>> convert_mincutset_to_probaset (std::vector<std::vector<int>> min_cutsets){
        std::vector<std::vector<int>> prob_sets;
        prob_sets.push_back(min_cutsets[0]);

        for (size_t i = 0; i < min_cutsets.size(); i++)
        {
            // spilt the min_cutset into two parts: one selected set and the remaining sets
            std::vector<int> selected_set = min_cutsets[i];
            std::vector<std::vector<int>> remaining_sets(min_cutsets.begin() + i + 1, min_cutsets.end());
            
            for (const auto& set : remaining_sets)
            {
                std::vector<std::vector<int>> disjoint_sets = create_disjoint_set(selected_set, set);
                

                //TODO: check the absorption rule : x + xy = x
                for (const auto& disjoint_set : disjoint_sets)
                {
                    if (std::find(prob_sets.begin(), prob_sets.end(), disjoint_set) == prob_sets.end()) {
                        prob_sets.push_back(disjoint_set);
                    }
                }
            }
            
            
        }
        return min_cutsets;
    }

}

int main() {
    using namespace rbd_bool;
    using namespace rbd_utility;
    std::map<std::string, int> alphabet_int_map = create_alphabet_int_map();
    std::map<int, std::string> int_alphabet_map = create_int_alphabet_map();
    std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../minimal_cut_set.json");
    // for (const auto &min_cutset : min_cutsets)
    // {
    //     std::cout << "src: " << min_cutset.src_dst.first << " dst: " << min_cutset.src_dst.second << std::endl;
    //     for (const auto &cutset : min_cutset.min_cutsets)
    //     {
    //         for (const auto &node : cutset)
    //         {
    //             std::cout << node << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }
    std::vector<int> set1 = {1, -3, 4};
    std::vector<int> set2 = {-1, 2, 3};
    std::sort(set1.begin(), set1.end());
    std::sort(set2.begin(), set2.end());
    std::vector<std::vector<int>> vec_of_vec = create_disjoint_set(set1, set2);
    for (const auto &vec : vec_of_vec)
    {
        for (const auto &elem : vec)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }

}