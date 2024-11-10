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

        // create disjoint sets
        for (size_t i = 0; i < RC.size(); i++)
        {   
            // add the element to set1, the last element to add is negative
            int elem_to_add = (i == RC.size() - 1) ? -RC[i] : RC[i];
            set2.push_back(elem_to_add);
            // save the new set
            result.push_back(set2);
        }

        return result;
    }

    std::vector<std::vector<int>> convert_mincutset_to_probaset (std::vector<std::vector<int>> min_cutsets){
        // TODO: implement this function
        return NULL;
    }

}

int main() {
    // using namespace rbd_bool;
    // std::vector<MinCutset> min_cutsets = read_minimal_cut_set("../minimal_cut_set.json");
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
}