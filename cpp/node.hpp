// node.hpp
#ifndef NODE_HPP
#define NODE_HPP

#include <algorithm>
#include <string>
#include <vector>

namespace alphageometry
{

    class Dependency; // forward declaration

    class Node
    {
    public:
        std::string name;
        Node *rep_by;
        std::vector<Node *> neighbors;
        // Change merge_history to store pointers to Dependency.
        std::vector<Dependency*> merge_history;

        Node(const std::string &name) : name(name), rep_by(nullptr) {}
        virtual ~Node();

        virtual std::string to_string() const = 0;

        Node *rep()
        {
            return rep_by ? rep_by->rep() : this;
        }

        void add_neighbor(Node *other, const Dependency *deps = nullptr)
        {
            if (other)
            {
                // Use std::find now that <algorithm> is included.
                if (std::find(neighbors.begin(), neighbors.end(), other) == neighbors.end())
                    neighbors.push_back(other);
                if (deps)
                    merge_history.push_back(const_cast<Dependency*>(deps));
            }
        }

        void merge_with(Node *other, const Dependency &deps);
    };

} // namespace alphageometry

// Include the header that contains the full definition of Dependency.
// Since graph.hpp defines Dependency and also includes node.hpp,
// the include guards will prevent circular re-inclusion.
// #include "graph.hpp"

#endif // NODE_HPP
