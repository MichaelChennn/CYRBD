#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <igraph.h>
#include <unordered_set>
#include <set>
#include <rbd_utility.h>

namespace rbd
{
    struct RBD_node
    {
        float avail;
        igraph_t graph;
        RBD_node* left;
        RBD_node* right;

        RBD_node(float avail, igraph_t graph) : avail(avail), graph(graph), left(nullptr), right(nullptr) {}
    };

    // Converting json graph file to igraph_t 
    igraph_t json_to_igraph(std::string file_path);

    std::vector <int> read_minimal_cut_set(std::string file_path, int src, int dst);

    bool is_subset(const std::unordered_set<int>& set1, const std::set<int>& set2);

    std::pair<int, int> create_subgraph(igraph_t& graph, std::unordered_set<int> &unavail_nodes, std::unordered_set<int> &avail_nodes);

    int find_path_DFS(igraph_t& graph, int src, int dst ,std::unordered_set <int> &avail_nodes, std::unordered_set <int> &unavail_nodes, std::set<std::set<int>> &minimal_cut_set);

    int find_path_Dijkstra(igraph_t& graph, int src, int dst ,std::unordered_set <int> &avail_nodes, std::unordered_set <int> &unavail_nodes, std::set<std::set<int>> &minimal_cut_set);

    int find_path_Bellman_Ford(igraph_t& graph, int src, int dst ,std::unordered_set <int> &avail_nodes, std::unordered_set <int> &unavail_nodes, std::set<std::set<int>> &minimal_cut_set);

}

