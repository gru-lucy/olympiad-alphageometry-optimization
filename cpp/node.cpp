#include "node.hpp"
#include "graph.hpp" // This makes the full definition of Dependency available.

namespace alphageometry
{

    Node::~Node()
    {
        for (auto dep : merge_history)
        {
            delete dep;
        }
    }

} // namespace alphageometry
