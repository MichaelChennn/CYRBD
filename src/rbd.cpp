#include "rbd.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <igraph.h>

using json = nlohmann::json;

namespace rbd
{

igraph_t json_to_igraph(std::string file_path) {
    // open the json file
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        exit(1);
    }

    // read the json file
    json j_graph = json::parse(file);

    // close the file
    file.close();

    // parse the nodes and edges
    std::vector<int> nodes = j_graph["nodes"].get<std::vector<int>>();
    std::vector<std::pair<int, int>> edges;
    for (const auto& edge : j_graph["edges"]) {
        edges.emplace_back(edge[0], edge[1]);
    }

    // create the igraph_t object (undirected graph)
    igraph_t graph;
    if (igraph_empty(&graph, nodes.size(), IGRAPH_UNDIRECTED) != IGRAPH_SUCCESS) {
        std::cerr << "Cannot create igraph object" << std::endl;
        exit(1);
    }

    // add the edges to the graph
    for (const auto& edge : edges) {
        if (igraph_add_edge(&graph, edge.first, edge.second) != IGRAPH_SUCCESS) {
            std::cerr << "Cannot add edge to graph" << std::endl;
            exit(1);
        }
    }
    // print the graph information
    std::cout << "Graph created with " << igraph_vcount(&graph) << " vertices and " << igraph_ecount(&graph) << " edges." << std::endl;

    return graph;
    }
}

 int main() {
    // convert the json file to igraph_t object
    igraph_t graph = rbd::json_to_igraph("../simple_graph.json");

    igraph_vector_int_t vector, edge;
    igraph_vector_int_init(&vector, 0);
    igraph_vector_int_init(&edge, 0);
    igraph_get_shortest_path(&graph, &vector, &edge, 0, 6, IGRAPH_ALL);

    std::cout << "Shortest path from " << 0 << " to " << 6 << ": ";
    for (int i = 0; i < igraph_vector_int_size(&vector); i++) {
        std::cout << VECTOR(vector)[i] << " ";
    }
    std::cout << std::endl;
    // destroy the graph
    igraph_destroy(&graph);
    return 0;

}




