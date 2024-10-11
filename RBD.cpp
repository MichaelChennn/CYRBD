#include "RBD.h"

namespace RBD
{
    int add(int a, int b) {
        return a + b;
    }

    RBD_node* create_RBD_tree(float avail) {
        return new RBD_node(avail);
    }
}



