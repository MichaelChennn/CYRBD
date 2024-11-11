#include "rbd.h"

using json = nlohmann::json;

namespace rbd
{

    igraph_t json_to_igraph(std::string file_path)
    {

        // open the json file
        std::ifstream file(file_path);
        if (!file.is_open())
        {
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
        for (const auto &edge : j_graph["edges"])
        {
            edges.emplace_back(edge[0], edge[1]);
        }

        // create the igraph_t object (undirected graph)
        igraph_t graph;
        if (igraph_empty(&graph, nodes.size(), IGRAPH_UNDIRECTED) != IGRAPH_SUCCESS)
        {
            std::cerr << "Cannot create igraph object" << std::endl;
            exit(1);
        }

        // add the edges to the graph
        for (const auto &edge : edges)
        {
            if (igraph_add_edge(&graph, edge.first, edge.second) != IGRAPH_SUCCESS)
            {
                std::cerr << "Cannot add edge to graph" << std::endl;
                exit(1);
            }
        }
        // print the graph information
        std::cout << "Graph created with " << igraph_vcount(&graph) << " vertices and " << igraph_ecount(&graph) << " edges." << std::endl;

        return graph;
    }

    // check if set1 is a subset of set2
    bool is_subset(const std::set<int> &set1, const std::unordered_set<int> &set2)
    {
        for (const int &elem : set1)
        {
            if (set2.find(elem) == set2.end())
            {
                return false;
            }
        }
        return true;
    }

    bool is_minimal_cut_set_covered(std::unordered_set<int> unavailable_nodes, std::set<std::set<int>> minimal_cut_set)
    {
        // if one minimal cut set are contained in the unavailable_nodes, return true
        for (const auto &cut_set : minimal_cut_set)
        {
            if (is_subset(cut_set, unavailable_nodes))
            {
                return true;
            }
        }
        return false;
    }

    std::pair<int, int> create_subgraph(igraph_t &graph, int src, int dst, std::unordered_set<int> &avail_nodes, std::unordered_set<int> &unavail_nodes)
    {
        // add edges from
        if (!avail_nodes.empty())
        {
            for (const int &node : avail_nodes)
            {
                igraph_vector_int_t neighbors;
                igraph_vector_int_init(&neighbors, 0);
                igraph_neighbors(&graph, &neighbors, node, IGRAPH_ALL);

                for (int i = 0; i < igraph_vector_int_size(&neighbors); i++)
                {
                    for (int j = i + 1; j < igraph_vector_int_size(&neighbors); j++)
                    {
                        int from = VECTOR(neighbors)[i];
                        int to = VECTOR(neighbors)[j];
                        igraph_integer_t eid;
                        igraph_get_eid(&graph, &eid, from, to, IGRAPH_UNDIRECTED, 0);
                        if (from != to && eid < 0)
                        {
                            igraph_add_edge(&graph, from, to);
                            std::cout << "Added edge: " << from << " - " << to << std::endl;
                        }
                    }
                }
                igraph_vector_int_destroy(&neighbors);
                unavail_nodes.insert(node);
            }
        }

        utility::export_to_graphviz(graph, "subgraph.dot");

        if (unavail_nodes.empty())
        {
            return std::make_pair(src, dst);
        }
        else
        {
            // delete the unavailable nodes from the graph
            igraph_vector_int_t idx;
            igraph_vector_int_init(&idx, 0);
            igraph_vector_int_t vec;
            igraph_vector_int_init(&vec, unavail_nodes.size());
            int i = 0;
            for (const int &node : unavail_nodes)
            {
                VECTOR(vec)
                [i] = node;
                i++;
            }
            igraph_vs_t vs;
            igraph_vs_vector(&vs, &vec);
            igraph_delete_vertices_idx(&graph, vs, &idx, NULL);
            // return the new src and new dst pair
            return std::make_pair(VECTOR(idx)[src] - 1, VECTOR(idx)[dst] - 1);
        }
    }

    bool DFS(int src, int target, std::vector<bool> &visited, const igraph_t &graph, int &path_count, int parent = -1)
    {

        // mark the current node as visited
        visited[src] = true;
        std::cout << "Visited: " << src << std::endl;

        // if the target is reached, return true
        if (src == target)
        {
            std::cout << "Target reached by " << parent << std::endl;
            path_count++;
            std::cout << "Path count: " << path_count << std::endl;
            visited[src] = false;
            return true;
        }

        // iterate over the neighbors
        igraph_vector_int_t neighbors;
        igraph_vector_int_init(&neighbors, 0);
        igraph_neighbors(&graph, &neighbors, src, IGRAPH_ALL);

        for (int i = 0; i < igraph_vector_int_size(&neighbors); i++)
        {
            int neighbor = VECTOR(neighbors)[i];
            if (neighbor != parent && !visited[neighbor])
            {
                if (DFS(neighbor, target, visited, graph, path_count, src))
                {
                    if (path_count > 1)
                    {
                        igraph_vector_int_destroy(&neighbors);
                        return true;
                    }
                }
            }
        }

        igraph_vector_int_destroy(&neighbors);
        return false;
    }

    int find_path_DFS(igraph_t& graph, int src, int dst, std::unordered_set<int> &avail_nodes, std::unordered_set<int> &unavail_nodes, std::set<std::set<int>> &minimal_cut_set)
    {
        // if one minimal cut set are contained in the unavailable_nodes, 0 path is found
        if (is_minimal_cut_set_covered(unavail_nodes, minimal_cut_set))
        {
            std::cout << "Minimal cut set is covered" << std::endl;
            return 0;
        }
        // TODO: create subgraph
        std::pair<int, int> new_src_dst = create_subgraph(graph, src, dst, avail_nodes, unavail_nodes);
        src = new_src_dst.first;
        dst = new_src_dst.second;

        // use DFS to find the path
        std::vector<bool> visited(igraph_vcount(&graph), false);
        int path_count = 0;
        DFS(src, dst, visited, graph, path_count);

        if (path_count == 0)
        {
            return 0;
        }
        else if (path_count == 1)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

}

int main()
{
    using namespace rbd;
    using namespace rbd_utility;
    igraph_t graph = rbd::json_to_igraph("../simple_graph.json");

    // aC
    std::unordered_set<int> avail_nodes = {1,2};
    std::unordered_set<int> unavail_nodes = {3};
   
    // dot -Tpng graph.dot -o graph.png
    std::set<std::set<int>> minimal_cut_set = {
        {1,3},
        {2,4},
        {2,5,3},
        {1,5,4}
    };

    int path = find_path_DFS(graph, 0, 6, avail_nodes, unavail_nodes, minimal_cut_set);
    std::cout << "Path: " << path << std::endl;
    export_to_graphviz(graph, "graph.dot");
    return 0;
}
