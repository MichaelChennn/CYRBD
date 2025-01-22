#include "rbd.h"

using json = nlohmann::json;


namespace rbd
{
    std::vector<std::vector<int>> makeDisjointSet(std::vector<int> set1, std::vector<int> set2)
    {
        // Check disjointness of set1 and set2: if x in set1 and x' in set2, set1 and set2 are disjoint
        // RC set: the elements in set1 but not in set2
        std::vector<int> RC;
        for (const int &elem : set1) {
            if (std::find(set2.begin(), set2.end(), -elem) != set2.end()) {
                return {set2};
            }
            if (std::find(set2.begin(), set2.end(), elem) == set2.end()) {
                RC.push_back(elem);
            }
        }

        // Absorption rule: x + xy = x, if the RC is empty return an empty vector
        if (RC.empty())
        {
            return std::vector<std::vector<int>>{};
        }

        // Save the result
        std::vector<std::vector<int>> result;

        for (size_t i = 0; i < RC.size(); i++)
        {   
            // every time add one more element from RC to set2 and the element to add is negative.
            set2.push_back(-RC[i]);
            // save the new set
            result.push_back(set2);
            // inverse the last element to keep only the last element negative
            set2.back() = -set2.back();
        }

        return result;
    }

    std::vector<std::vector<int>> minCutSetToProbaset(const int &src, const int &dst, std::vector<std::vector<int>> &mincutset)
    {
        std::vector<std::vector<int>> probSet;
        // std::vector<std::vector<int>> tempSet{mincutset.begin(), mincutset.end()};

        // remove the sets: {src} and {dst}
        auto it_src = std::find(mincutset.begin(), mincutset.end(), std::vector<int>{src});
        if (it_src != mincutset.end())
        {
            mincutset.erase(it_src);
        }
        auto it_dst = std::find(mincutset.begin(), mincutset.end(), std::vector<int>{dst});
        if (it_dst != mincutset.end())
        {
            mincutset.erase(it_dst);
        }
        if (mincutset.empty())
        {
            return {};
        }

        // inverse the min_cutsets
        for (auto &set : mincutset)
        {
            for (auto &elem : set)
            {
                elem = -elem;
            }
        }

        // convert the min_cutsets to the probability sets
        while (mincutset.size() > 0)
        {

            if (mincutset.size() == 1)
            {
                probSet.push_back(mincutset[0]);
                break;
            }
            std::vector<int> selected_set = mincutset.front();
            probSet.push_back(selected_set);
            std::vector<std::vector<int>> remaining_sets(mincutset.begin() + 1, mincutset.end());
            mincutset.clear();

            for (const auto &set : remaining_sets)
            {
                std::vector<std::vector<int>> disjoint_sets = makeDisjointSet(selected_set, set);
                for (const auto &disjoint_set : disjoint_sets)
                {
                    mincutset.push_back(disjoint_set);
                }
            }
        }

        return probSet;
    }

    double probasetToAvailability(const int &src, const int &dst, const ProbabilityArray &probArr, std::vector<std::vector<int>> &probSet)
    {
        // Save the final result
        double unavil = 0.0;
        for (const auto &set : probSet)
        {
            // Save the temp result
            double temp = 1.0;
            for (const auto &num : set)
            {
                temp *= probArr[num];
            }
            unavil += temp;
        }

        double avail = 1.0 - unavil;
        double result = probArr[src] * probArr[dst] * avail;

        return result;
    }
  
    double evaluateAvailability(const int &src, const int &dst, const ProbabilityArray& probArr, std::vector<std::vector<int>>& mincutset) {
        // evaluate the unavailability for the given src-dst pair
        try
        {   
            std::vector<std::vector<int>> probSet = minCutSetToProbaset(src, dst, mincutset);
            double availability = probasetToAvailability(src, dst, probArr, probSet);
            return availability;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return 0.0;
    }

    std::vector<std::tuple<int, int, double>> evaluateAvailabilityTopology(const std::vector<std::pair<int, int>>& nodePairs, const ProbabilityArray& probArr, std::vector<std::vector<std::vector<int>>>& mincutsets) {
        std::vector<std::tuple<int, int, double>> availabilities;
        int current = 0;
        try
        {
            for (auto &mincutset : mincutsets)
            {
                // evaluate the availability for the given src-dst pair
                int src = nodePairs[current].first;
                int dst = nodePairs[current].second;
                double availability = evaluateAvailability(src, dst, probArr, mincutset);
                availabilities.emplace_back(src, dst, availability);
                current++;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        return availabilities;
    }


    int boolExprCount(std::vector<std::vector<int>>& mincutset, const int &src, const int &dst) {
        std::vector<std::vector<int>> probSet = minCutSetToProbaset(src, dst, mincutset);
        // return the length of the probability set plus 2 for the src and dst
        return probSet.size() + 2;
    }

    std::vector<std::vector<int>> parseNestedVectors(const std::string& input) {
        std::vector<std::vector<int>> result;
        std::regex pattern(R"(\[([^\[\]]*)\])"); // 匹配方括号内的内容
        auto it = std::sregex_iterator(input.begin(), input.end(), pattern);
        auto end_it = std::sregex_iterator();

        for (; it != end_it; ++it) {
            std::string innerContent = (*it)[1]; // 提取方括号内的内容
            std::vector<int> tempVector;
            std::stringstream ss(innerContent);
            std::string num;

            // 按逗号分隔并转换为整数
            while (std::getline(ss, num, ',')) {
                tempVector.push_back(std::stoi(num) + 1); // 转换为整数
            }
            result.push_back(tempVector);
        }

        return result;
    }


}

int main() {
    using namespace rbd;
    // Test Germany_50 topology
    std::string topology = "Germany_50";
    std::string filePath = "../topologies/" + topology + "/mincutsets_" + topology + ".csv";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    // Skip the first line
    std::getline(file, line); 
   
    std::vector<std::vector<std::vector<int>>> mincutsets;

    while (std::getline(file, line)) {
        size_t pos = line.find(",");
        size_t pos2 = line.find(",", pos + 1);
        line = line.substr(pos2 + 1); 
        size_t pos3 = line.find("[");
        line = line.substr(pos3 + 1, line.size() - 4);
        
        std::vector<std::vector<int>> mincutset = parseNestedVectors(line);
        mincutsets.push_back(mincutset);
    }

    file.close();

    //  Output the result for verification
    // for (const auto &mincutset : mincutsets) {
    //     for (const auto &set : mincutset) {
    //         for (const auto &elem : set) {
    //             if (elem == 0) {
    //             std::cout << "Find 0" << std::endl;
    //             }
    //             // std::cout << elem << " ";
    //         }
    //         // std::cout << std::endl;
    //     }
    //     // std::cout << std::endl;
    // }

    std::vector<std::pair<int, int>> nodePairs;
    std::pair<int, int> nodePair;
    for (int i = 1; i < 50; i++) {
        nodePair.first = i;
        for (int j = i + 1; j <= 50; j++) {
            nodePair.second = j;
            nodePairs.push_back(nodePair);
            // std::cout << nodePair.first << " " << nodePair.second << std::endl;
        }
        
    }

    std::map<int, double> prob;
    for (int i = 1; i <= 50; i++) {
        prob[i] = 0.9;
    }
    ProbabilityArray probArr(prob);

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::tuple<int, int, double>> results = evaluateAvailabilityTopology(nodePairs, prob, mincutsets);   
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;


    return 0;
    
}
