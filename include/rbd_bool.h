#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <unordered_set>
#include <set>
#include <utility>
#include <cmath>
#include <rbd_utility.h>

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
    /**
     * @brief Read trhe minimal cut set from the json file
     * 
     * @param file_path 
     * @return std::vector<MinCutset> sorted minimal cut sets
     */
    std::vector<MinCutset> read_minimal_cut_set(const std::string file_path);

    /**
     * @brief Read the probability array from the json file
     * 
     * @param file_path 
     * @return ProbabilityArray 
     */
    ProbabilityArray read_probability_array(const std::string file_path);

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
     * @return std::vector<std::vector<int>> sorted disjoint sets
     * 
     */
    std::vector<std::vector<int>> create_disjoint_set(std::vector<int> set1, std::vector<int> set2);

    /**
     * @brief Convert the path-sets to the probability sets
     * Algorithm:
     * For each min-cutset, create a couple of disjoint sets from the other min-cutset
     * 1. initialize the probability sets as the min-cutsets
     * 2. select the left most set as the selected_set and the remaining min-cutsets as the right remaining sets e.g min-cutsets = {set1, set2, set3}
     * then selected_set = set1, remaining sets = {set2, set3}
     * 3. create the disjoint sets from the set1 and each set in the remaining sets e.g. create_disjoint_set(set1, set2), create_disjoint_set(set1, set3)
     * 4. the new disjoint set should follow the rules:
     * Absorption: x + xy = x
     * Reduction: x + x'y = x + y
     *            xy + xy = xy
     *            xy + xy' = x
     * 5. add the new disjoint sets to the probability sets
     * 6. repeat the steps 2-5 for the second left most set and the remaining sets are the right remaining sets e.g. selected_set = set2, remaining sets = {set3}
     * @param path_sets
     * @return std::vector<std::vector<int>> probability sets
     */
    // std::vector<std::vector<int>> convert_pathset_to_probaset (std::vector<std::vector<int>>& path_sets);

    /**
     * @brief Convert the min-cutsets to the probability sets
     * Algorithm is same as the convert_pathset_to_probaset, only the result is inversed
     * @param min_cutsets 
     * @return std::vector<std::vector<int>> probability sets
     */
    std::vector<std::vector<int>> convert_mincutset_to_probaset (const std::vector<std::vector<int>>& min_cutsets, const std::pair<int, int>& src_dst);

    /**
     * @brief Compute the probability of the given probability set and the probability array
     * e.g. prob_set = {{1, 2, 3}, {1, 2, -3}}, prob_array = {0.1, 0.2, 0.3}
     * then the probability is 0.1 * 0.2 * 0.3 + 0.1 * 0.2 * 0.7 = 0.02
     * @param src_dst pair
     * @param prob_set 
     * @param prob_array 
     * @return the probability in double
     */
    // double compute_avail(std::pair<int, int> &src_dst, std::vector<std::vector<int>> &prob_set, ProbabilityArray &prob_array);

    double compute_avail(const std::pair<int, int> &src_dst, std::vector<std::vector<int>> &prob_set, const ProbabilityArray &prob_array);

    std::map<std::pair<int, int>, double> evaluate_avail(const std::string file_name);

    std::map<std::pair<int, int>, double> evaluate_unavail(const std::string file_name);

}