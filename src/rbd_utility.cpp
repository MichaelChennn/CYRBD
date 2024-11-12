#include "rbd_utility.h"

namespace rbd_utility {
    void export_to_graphviz(const igraph_t& graph, const std::string& file_path) {
        FILE* file = fopen(file_path.c_str(), "w");
        if (!file) {
            std::cerr << "can open file" << std::endl;
            exit(1);
        }

        igraph_write_graph_dot(&graph, file);
        fclose(file);
    }

    std::map<std::string, int> create_alphabet_int_map() {
        std::map<std::string, int> alphabet_map;
        for (int i = 0; i < 26; i++) {
            std::string s(1, 'a' + i);
            alphabet_map[s] = i + 1;
            alphabet_map[s + "'"] = -(i + 1);

        }
        return alphabet_map;
    }

    std::map<int, std::string> create_int_alphabet_map() {
        std::map<int, std::string> alphabet_map;
        for (int i = 0; i < 26; i++) {
            std::string s(1, 'a' + i);
            std::string s_prime = s + "'";
            alphabet_map[i + 1] = s;
            alphabet_map[-(i + 1)] = s_prime;
        }
        return alphabet_map;
    }

    void print_vector_int(const std::vector<int>& vec) {
        std::map<int, std::string> map = create_int_alphabet_map();
        for (const auto& elem : vec) {
            std::cout << map[elem] << " ";
        }
        std::cout << std::endl;
    }

    void print_vector_alpahbet(const std::vector<std::string>& vec) {
        std::map<std::string, int> map = create_alphabet_int_map();
        for (const auto& elem : vec) {
            std::cout << map[elem] << " ";
        }
        std::cout << std::endl;
    }
}