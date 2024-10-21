#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <igraph.h>


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


    int find_path(std::string file_path, int src, int dst ,std::vector <int> &avail_nodes);
}

