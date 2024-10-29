#include <iostream>
#include <fstream>
#include <igraph.h>

namespace utility {
void export_to_graphviz(const igraph_t& graph, const std::string& file_path);
}