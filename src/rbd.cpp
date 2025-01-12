#include "rbd.h"

using json = nlohmann::json;


namespace rbd
{

    // std::map<std::pair<int,int>, std::vector<std::vector<int>>> readMinCutSet(const std::string file_path)
    // {
    //     // open the json file
    //     std::ifstream
    //         file(file_path);

    //     if (!file.is_open())
    //     {
    //         std::cerr << "Error on reading minimal cut set file!" << std::endl;
    //         exit(1);
    //     }

    //     // create a vector of MinCutset
    //     std::map<std::pair<int,int>, std::vector<std::vector<int>>> min_cut_sets;

    //     // read the json file
    //     json j = json::parse(file);
    //     if (j.contains("minimal_cutsets"))
    //     {
    //         for (auto &it :j["minimal_cutsets"]) {
    //             std::pair<int, int> src_dst = {it["src-dst"][0], it["src-dst"][1]};
    //             std::vector<std::vector<int>> tmp_min_cutsets;
    //             for (auto &min_cutset : it["min-cutsets"]) {
    //                 std::vector<int> cutsetVec(min_cutset.begin(), min_cutset.end());
    //                 std::sort(cutsetVec.begin(), cutsetVec.end());
    //                 tmp_min_cutsets.push_back(cutsetVec);
    //             }
    //             min_cut_sets[src_dst] = tmp_min_cutsets;
    //         }
    //     }
    //     else
    //     {
    //         std::cerr << "Error on reading minimal cut sets from the json file!" << std::endl;
    //     }

    //     // close the file
    //     file.close();

    //     return min_cut_sets;
    // }

    // ProbabilityArray readProbabilityArray(const std::string file_path)
    // {
    //     // open the json file
    //     std::ifstream file(file_path);
    //     if (!file.is_open())
    //     {
    //         std::cerr << "Error on opening probabilty file" << std::endl;
    //         exit(1);
    //     }

    //     // read the json file
    //     json j = json::parse(file);
    //     std::vector<double> pos_array = j["probability"].get<std::vector<double>>();

    //     // create the ProbabilityArray object
    //     ProbabilityArray prob_array(pos_array);

    //     // close the file
    //     file.close();

    //     return prob_array;
    // }

    std::vector<std::vector<int>> makeDisjointSet(std::vector<int> set1, std::vector<int> set2)
    {
        // check if the set1 and set2 are already disjoint
        // with the rule：when x in set1 and x' in set2, set1 and set2 are disjoint
        // take care of the sepcial case: -0 is represented by INT32_MIN
        for (const int &elem : set1)
        {
            if (elem == 0)
            {
                std::cerr << "Error on handling node 0!" << std::endl;
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

            set2_copy.back() = -set2_copy.back();
            
            // sort the set to be added
            std::sort(set2_copy.begin(), set2_copy.end());

            // save the new set
            result.push_back(set2_copy);
        }

        return result;
    }

    std::vector<std::vector<int>> minCutSetToProbaset(const int &src, const int &dst, const std::vector<std::vector<int>> &min_cutsets)
    {
        std::vector<std::vector<int>> prob_sets;
        std::vector<std::vector<int>> temp_sets{min_cutsets.begin(), min_cutsets.end()};

        // remove the sets: {src} and {dst}
        auto it_src = std::find(temp_sets.begin(), temp_sets.end(), std::vector<int>{src});
        if (it_src != temp_sets.end())
        {
            temp_sets.erase(it_src);
        }
        auto it_dst = std::find(temp_sets.begin(), temp_sets.end(), std::vector<int>{dst});
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
                elem = -elem;
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
                std::vector<std::vector<int>> disjoint_sets = makeDisjointSet(selected_set, set);
                for (const auto &disjoint_set : disjoint_sets)
                {
                    temp_sets.push_back(disjoint_set);
                }
            }
        }

        return prob_sets;
    }

    double probasetToAvailability(const int &src, const int &dst, const ProbabilityArray &prob_array, std::vector<std::vector<int>> &prob_set)
    {
        // Save the final result
        double unavil = 0.0;
        for (const auto &set : prob_set)
        {
            // Save the temp result
            double temp = 1.0;
            for (const auto &num : set)
            {
                temp *= prob_array[num];
            }
            unavil += temp;
        }

        double avail = 1.0 - unavil;
        double avail_total = prob_array[src] * prob_array[dst] * avail;

        return avail_total;
    }

    // std::map<std::pair<int, int>, double> evaluateAvailabilityTopology_old(const std::string file_name)
    // {

    //     // read the minimal cut set and the probability array
    //     std::map<std::pair<int,int>, std::vector<std::vector<int>>> mincutsets_map = readMinCutSet(file_name);
    //     ProbabilityArray prob_array = readProbabilityArray(file_name);

    //     // save the result
    //     std::map<std::pair<int, int>, double> result;

    //     // evaluate the unavailability for each src-dst pair
    //     for (const auto &mincutsets : mincutsets_map)
    //     {
    //         std::vector<std::vector<int>> prob_sets = minCutSetToProbaset(mincutsets.first.first, mincutsets.first.second, mincutsets.second);
    
    //         double availability= probasetToAvailability(mincutsets.first.first, mincutsets.first.second, prob_array, prob_sets);
        
    //         result[mincutsets.first] = availability;
    //     }
    //     return result;
    // }

    // double evaluateAvailability_old(const std::string file_name, const int &src, const int &dst) {
    //     // read the minimal cut set and the probability array
    //     std::map<std::pair<int,int>, std::vector<std::vector<int>>> mincutsets_map = readMinCutSet(file_name);
    //     ProbabilityArray prob_array = readProbabilityArray(file_name);
    //     // evaluate the unavailability for the given src-dst pair
    //     try
    //     {   
    //         std::pair<int, int> src_dst = {src, dst};
    //         std::vector<std::vector<int>> prob_sets = minCutSetToProbaset(src, dst, mincutsets_map[src_dst]);
    //         double availability = probasetToAvailability(src, dst, prob_array, prob_sets);
    //         return availability;
    //     }
    //     catch(const std::exception& e)
    //     {
    //         std::cerr << e.what() << '\n';
    //     }
    //     return 0.0;
    // }

    double evaluateAvailability(const std::vector<std::vector<int>>& min_cutsets, const ProbabilityArray& prob_array, const int &src, const int &dst) {
        // evaluate the unavailability for the given src-dst pair
        try
        {   
            std::vector<std::vector<int>> prob_sets = minCutSetToProbaset(src, dst, min_cutsets);
            double availability = probasetToAvailability(src, dst, prob_array, prob_sets);
            return availability;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return 0.0;
    }

    // void writeResultToFile(const std::string topologie_name, const std::map<std::pair<int, int>, double> &result) {
    //     // create the result file
    //     std::string file_path = "../results/" + topologie_name + "_availability_cpp.csv";
    //     std::ofstream file(file_path);
    //     if (!file.is_open()) {
    //         std::cerr << "Error on saving result!" << std::endl;
    //         exit(1);
    //     }

    //     // write the result to the file
    //     file << "source,target,availability" << std::endl;
    //     for (const auto &pair : result) {
    //         // remove the trailing zeros
    //         std::ostringstream out;
    //         out << std::fixed << std::setprecision(9) << pair.second;
    //         std::string avilability = out.str();
    //         avilability.erase(avilability.find_last_not_of('0') + 1, std::string::npos);
    //         if (avilability.back() == '.') {
    //             avilability.pop_back();
    //         }
    //         // write the result to the csv file
    //         file << pair.first.first << "," << pair.first.second << ",";
    //         file << std::fixed << std::setprecision(9) << avilability << std::endl;
    //     }

    //     // close the file
    //     file.close();
    // }

    int lengthOfProbaset(const std::vector<std::vector<int>>& min_cutsets, const int &src, const int &dst) {
        std::vector<std::vector<int>> prob_sets = minCutSetToProbaset(src, dst, min_cutsets);
        return prob_sets.size();
    }

}
