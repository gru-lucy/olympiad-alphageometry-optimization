// graph.hpp
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "node.hpp" 
#include "geometry.hpp"
#include "problem.hpp"
#include "numericals.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace alphageometry
{

    class Dependency; // forward declaration

    using DependencyList = std::vector<Dependency>;

    class Graph
    {
    public:
        // Map names to nodes.
        std::unordered_map<std::string, Node *> _name2node;

        Graph();
        ~Graph();

        // Build a problem into a graph.
        std::pair<Graph, DependencyList> build_problem(const Problem &p,
                                                       const std::unordered_map<std::string, Definition> &defs, bool verbose = false);

        // Minimal add_piece function (only a few predicates are implemented).
        DependencyList add_piece(const std::string &name, const std::vector<Point *> &args,
                                 const Dependency &deps);

        bool check(const std::string &name, const std::vector<Point *> &args) const;

        void connect(Node *a, Node *b, const Dependency *deps);

        Node *merge(const std::vector<Node *> &nodes, const Dependency &deps);

        std::vector<Point *> all_points() const;

        std::vector<Point *> names2nodes(const std::vector<std::string> &names) const;
    };

    class Dependency : public Node
    {
    public:
        std::string rule_name;
        int level;
        std::vector<Dependency*> why;

        Dependency(const std::string &name, const std::vector<Point *> &args,
                   const std::string &rule_name, int level)
            : Node(name), rule_name(rule_name), level(level)
        {
            // Cast Point* to Node* when adding neighbors
            for (auto *point : args)
            {
                // Assuming Point is derived from Node or has a way to convert to Node*
                add_neighbor(static_cast<Node *>(point));
            }
        }

        // Implement the pure virtual function from Node
        std::string to_string() const override
        {
            // Provide a basic implementation
            return "Dependency: " + name + " (Rule: " + rule_name + ")";
        }

        // Add copy constructors and assignment operators
        Dependency(const Dependency &other)
            : Node(other),
              rule_name(other.rule_name),
              level(other.level),
              why(other.why)
        {
        }

        Dependency &operator=(const Dependency &other)
        {
            if (this != &other)
            {
                Node::operator=(other);
                rule_name = other.rule_name;
                level = other.level;
                why = other.why;
            }
            return *this;
        }

        std::tuple<std::string, std::vector<std::string>> hashed(bool rename = false) const;
    };
} // namespace alphageometry

#endif // GRAPH_HPP
