#include <iostream>
#include <fstream>
#include <igraph.h>
#include <map>
#include <vector>

namespace rbd_utility {
void export_to_graphviz(const igraph_t& graph, const std::string& file_path);

std::map<std::string, int> create_alphabet_int_map();

std::map<int, std::string> create_int_alphabet_map();

void print_vector_int(const std::vector<int>& vec);

void print_vector_alpahbet(const std::vector<std::string>& vec);

}