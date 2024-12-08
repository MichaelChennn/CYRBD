#include <iostream>
#include <fstream>
#include <map>
#include <vector>

namespace rbd_utility {

std::map<std::string, int> create_alphabet_int_map();

std::map<int, std::string> create_int_alphabet_map();

void print_vector_int(const std::vector<int>& vec);

void print_vector_alpahbet(const std::vector<std::string>& vec);

void print_vector_of_vector_int(const std::vector<std::vector<int>>& vec);

std::pair<int, int> reverse_src_dst(const std::pair<int, int>& src_dst);

}