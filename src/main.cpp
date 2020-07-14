#include <bdsg/hash_graph.hpp>

#include <iostream>

int main(int argc, char** argv) {
    bdsg::HashGraph g;
    g.create_handle("A");
    std::cout << "Example graph node count: " << g.get_node_count() << std::endl;
}
