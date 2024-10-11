#include <igraph.h>

namespace RBD
{
    struct RBD_node
    {
        float avail;
        igraph_t graph;
        RBD_node* left;
        RBD_node* right;

        RBD_node(float avail, igraph_t graph) : avail(avail), graph(graph), left(nullptr), right(nullptr) {}
    };
    
    RBD_node* create_RBD_tree(float avail, igraph_t graph);

    int add(int a, int b);  
}

