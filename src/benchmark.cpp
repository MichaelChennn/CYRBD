#include "benchmark.h"
#include <chrono>


using namespace rbd;

namespace benchmark {
    void measure_time_topology(const std::string topology_name) {
        std::string file_path = "../topologies/" + topology_name + "/" + topology_name + ".json";
        auto start = std::chrono::high_resolution_clock::now();
        evaluateAvailabilityTopology(file_path);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;
    }

    void measure_time_topology_src_dst(const std::string topology_name, const std::pair<int, int> &src_dst) {
        std::string file_path = "../topologies/" + topology_name + "/" + topology_name + ".json";
        auto start = std::chrono::high_resolution_clock::now();
        evaluateAvailability(file_path, src_dst);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;
    }
}

int main () {
    using namespace benchmark;
    measure_time_topology("Germany_17");
    // measure_time_topology_src_dst("../topologies/Abilene/Abilene.json", {0, 6});
    return 0;
}