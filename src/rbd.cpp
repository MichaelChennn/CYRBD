#include "rbd.h"

using json = nlohmann::json;


namespace rbd
{
    std::vector<std::vector<int>> makeDisjointSet(std::vector<int> set1, std::vector<int> set2)
    {
        // check if the set1 and set2 are already disjoint
        // when x in set1 and x' in set2, set1 and set2 are disjoint
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

        // Absorption rule: x + xy = x, check if the RC is empty
        if (RC.empty())
        {
            return std::vector<std::vector<int>>{};
        }

        // create disjoint sets
        for (size_t i = 0; i < RC.size(); i++)
        {   

            // every time add one more element from RC to set2 and the last element to add is negative.
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

    int getBoolExprLen(const std::vector<std::vector<int>>& min_cutsets, const int &src, const int &dst) {
        std::vector<std::vector<int>> prob_sets = minCutSetToProbaset(src, dst, min_cutsets);
        // return the length of the probability set plus 2 for the src and dst
        return prob_sets.size() + 2;
    }

}
