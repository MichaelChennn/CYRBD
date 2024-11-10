#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <igraph.h>
#include <unordered_set>
#include <set>
#include <utility>
#include <cmath>

namespace rbd_bool
{

    struct MinCutset
    {
        std::pair<int, int> src_dst;
        std::vector<std::vector<int>> min_cutsets;
    };

    class ProbabilityArray
    {
    private:
        std::vector<double> pos_array;
        std::vector<double> neg_array;

    public:
        ProbabilityArray(const std::vector<double> &pos_arr) : pos_array(pos_arr), neg_array(pos_arr.size())
        {
            for (size_t i = 0; i < pos_arr.size(); ++i)
            {
                neg_array[i] = 1.0 - pos_arr[i];
            }
        }

        double operator[](int i) const
        {
            // special case for INT32_MIN
            // INT32_MIN is for -0
            if (i == INT32_MIN)
            {
                return neg_array[0];
            }
            if (std::abs(i) > pos_array.size())
            {
                throw std::out_of_range("Index out of range");
            }
            if (i >= 0)
            {
                return pos_array[i];
            }
            else
            {
                return neg_array[-i];
            }
        }

        void print() const
        {
            std::cout << "Positive Array: ";
            for (double elem : pos_array)
            {

                std::cout << elem << " ";
            }
            std::cout << std::endl;

            std::cout << "Negative Array: ";
            for (double elem : neg_array)
            {
                std::cout << elem << " ";
            }
            std::cout << std::endl;
        }
    };

    std::vector<MinCutset> read_minimal_cut_set(std::string file_path);

    ProbabilityArray read_probability_array(std::string file_path);

    /**
     * @brief Create a couple of disjoint sets from set2
     * Algorithm:
     * 1. Find the set: set1 \ set2, e.g. set1 = {1, 2, 3, 4, 5}, set2 = {2, 4}, then RC = set1 \ set2 = {1, 3, 5}
     * 2. add the elements in RC to set2 to create several new sets. The rule is every time add one more element from RC to set2 and the last element to add is negative.
     *      1) first step: {2, 4} -> {2, 4, 1}
     *      2) second step: {2, 4, 1} -> {2, 4, 1, -3}
     *      3) third step: {2, 4, 1, -3} -> {2, 4, 1, -3, -5}
     * 3. return the new disjoint sets {{2, 4, 1}, {2, 4, 1, -3}, {2, 4, 1, 3, -5}}
     * @param set1 
     * @param set2 
     * @return std::vector<std::vector<int>>
     * 
     */
    std::vector<std::vector<int>> create_disjoint_set(std::vector<int> set1, std::vector<int> set2);

    
    std::vector<std::vector<int>> convert_mincutset_to_probaset (std::vector<std::vector<int>> min_cutsets);

}