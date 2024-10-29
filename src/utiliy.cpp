#include "utility.h"

namespace utility {
    void export_to_graphviz(const igraph_t& graph, const std::string& file_path) {
        FILE* file = fopen(file_path.c_str(), "w");
        if (!file) {
            std::cerr << "can open file" << std::endl;
            exit(1);
        }

        igraph_write_graph_dot(&graph, file);
        fclose(file);
}
}