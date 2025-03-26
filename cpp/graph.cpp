#ifndef CONSTRUCTION_RULE
#define CONSTRUCTION_RULE "construction_rule"
#endif

#include "graph.hpp"
#include "problem.hpp"
#include "ddar.hpp"
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <iostream>

namespace alphageometry
{

    // ---------- Node Methods ----------
    // void Node::add_neighbor(Node *other, const Dependency *deps)
    // {
    //     if (other)
    //     {
    //         // Avoid adding duplicate neighbors
    //         auto it = std::find(neighbors.begin(), neighbors.end(), other);
    //         if (it == neighbors.end())
    //         {
    //             neighbors.push_back(other);
    //         }

    //         // If deps is provided, store it in merge history
    //         if (deps)
    //         {
    //             merge_history.push_back(*deps);
    //         }
    //     }
    // }

    void Node::merge_with(Node *other, const Dependency &deps)
    {
        other->rep_by = this;
        for (Node *nb : other->neighbors)
        {
            if (std::find(neighbors.begin(), neighbors.end(), nb) == neighbors.end())
            {
                neighbors.push_back(nb);
            }
        }
        merge_history.push_back(new Dependency(deps));
    }

    // ---------- Dependency Methods ----------
    // Dependency::Dependency(const std::string &name, const std::vector<Point *> &args,
    //                        const std::string &rule_name, int level)
    //     : Node(name), rule_name(rule_name), level(level)
    // {
    //     // For Dependency, we simply copy the point pointers as string names.
    //     for (auto p : args)
    //     {
    //         // We assume p->name is already the minimal name.
    //         // For our minimal implementation, we store the pointer in the neighbors vector.
    //         neighbors.push_back(p);
    //     }
    // }

    std::tuple<std::string, std::vector<std::string>> Dependency::hashed(bool rename) const
    {
        // Return a tuple (name, sorted list of argument names)
        std::vector<std::string> names;
        for (Node *p : neighbors)
        {
            names.push_back(p->name);
        }
        std::sort(names.begin(), names.end());
        return std::make_tuple(name, names);
    }

    // ---------- Graph Methods ----------
    Graph::Graph() {}

    Graph::~Graph()
    {
        for (auto &pair : _name2node)
        {
            delete pair.second;
        }
    }

    std::pair<Graph, DependencyList> Graph::build_problem(
        const Problem &p,
        const std::unordered_map<std::string, Definition> &defs,
        bool verbose)
    {
        Graph g;
        DependencyList added;
        // For each clause, process constructions
        for (const auto &clause : p.clauses)
        {
            // For our minimal implementation, we assume clause.points are used as-is.
            for (const auto &cons : clause.constructions)
            {
                auto def_it = defs.find(cons.name);
                if (def_it == defs.end())
                {
                    throw std::runtime_error("Definition not found for " + cons.name);
                }
                // For simplicity, we do not perform full translation.
                // Create a dependency with the construction name.
                Dependency dep(cons.name, g.names2nodes(clause.points), CONSTRUCTION_RULE, 0);
                // Call add_piece based on construction name.
                DependencyList deps = g.add_piece(cons.name, g.names2nodes(clause.points), dep);
                // Append resulting dependencies.
                added.insert(added.end(), deps.begin(), deps.end());
            }
        }
        return {g, added};
    }

    DependencyList Graph::add_piece(const std::string &name,
                                    const std::vector<Point *> &args,
                                    const Dependency &deps)
    {
        DependencyList out;
        if (name == "coll")
        {
            if (args.size() < 3)
                return out;
            Line l(args[0], args[1]);
            bool ok = true;
            for (size_t i = 2; i < args.size(); i++)
            {
                if (!l.is_on(*args[i]))
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
                out.push_back(deps);
        }
        else if (name == "para")
        {
            if (args.size() == 4)
            {
                Line l1(args[0], args[1]);
                Line l2(args[2], args[3]);
                if (l1.is_parallel(l2))
                    out.push_back(deps);
            }
        }
        else if (name == "perp")
        {
            if (args.size() == 4)
            {
                Line l1(args[0], args[1]);
                Line l2(args[2], args[3]);
                if (l1.is_perp(l2))
                    out.push_back(deps);
            }
        }
        else if (name == "cong")
        {
            if (args.size() == 4)
            {
                double d1 = args[0]->distance(*args[1]);
                double d2 = args[2]->distance(*args[3]);
                if (close_enough(d1, d2))
                    out.push_back(deps);
            }
        }
        else if (name == "midp")
        {
            if (args.size() == 3)
            {
                double d1 = args[0]->distance(*args[1]);
                double d2 = args[0]->distance(*args[2]);
                if (close_enough(d1, d2))
                    out.push_back(deps);
            }
        }
        else if (name == "circle")
        {
            if (args.size() == 4)
            {
                Point *o = args[0];
                double r1 = o->distance(*args[1]);
                double r2 = o->distance(*args[2]);
                double r3 = o->distance(*args[3]);
                if (close_enough(r1, r2) && close_enough(r2, r3))
                    out.push_back(deps);
            }
        }
        // Additional predicates can be added here.
        return out;
    }

    bool Graph::check(const std::string &name, const std::vector<Point *> &args) const
    {
        if (name == "coll")
        {
            if (args.size() < 3)
                return true;
            Line l(args[0], args[1]);
            for (size_t i = 2; i < args.size(); i++)
            {
                if (!l.is_on(*args[i]))
                    return false;
            }
            return true;
        }
        else if (name == "para")
        {
            if (args.size() != 4)
                return false;
            Line l1(args[0], args[1]);
            Line l2(args[2], args[3]);
            return l1.is_parallel(l2);
        }
        else if (name == "perp")
        {
            if (args.size() != 4)
                return false;
            Line l1(args[0], args[1]);
            Line l2(args[2], args[3]);
            return l1.is_perp(l2);
        }
        else if (name == "cong")
        {
            if (args.size() != 4)
                return false;
            double d1 = args[0]->distance(*args[1]);
            double d2 = args[2]->distance(*args[3]);
            return close_enough(d1, d2);
        }
        else if (name == "midp")
        {
            if (args.size() != 3)
                return false;
            double d1 = args[0]->distance(*args[1]);
            double d2 = args[0]->distance(*args[2]);
            return close_enough(d1, d2);
        }
        else if (name == "circle")
        {
            if (args.size() != 4)
                return false;
            Point *o = args[0];
            double r1 = o->distance(*args[1]);
            double r2 = o->distance(*args[2]);
            double r3 = o->distance(*args[3]);
            return (close_enough(r1, r2) && close_enough(r2, r3));
        }
        return false;
    }

    void Graph::connect(Node *a, Node *b, const Dependency *deps)
    {
        a->add_neighbor(b, deps);
        b->add_neighbor(a, deps);
    }

    Node *Graph::merge(const std::vector<Node *> &nodes, const Dependency &deps)
    {
        if (nodes.empty())
            return nullptr;
        Node *rep = nodes[0];
        for (size_t i = 1; i < nodes.size(); i++)
        {
            rep->merge_with(nodes[i], deps);
        }
        return rep;
    }

    std::vector<Point *> Graph::all_points() const
    {
        std::vector<Point *> pts;
        for (const auto &pair : _name2node)
        {
            Point *p = dynamic_cast<Point *>(pair.second);
            if (p)
                pts.push_back(p);
        }
        return pts;
    }

    std::vector<Point *> Graph::names2nodes(const std::vector<std::string> &names) const
    {
        std::vector<Point *> pts;
        for (const auto &n : names)
        {
            auto it = _name2node.find(n);
            if (it != _name2node.end())
            {
                Point *p = dynamic_cast<Point *>(it->second);
                if (p)
                    pts.push_back(p);
            }
            else
            {
                throw std::runtime_error("Node with name " + n + " not found.");
            }
        }
        return pts;
    }

} // namespace alphageometry

// ------------------------------
// Pybind11 module definition
// ------------------------------

#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <vector>
#include <string>

alphageometry::Problem *create_problem_from_txt(const std::string &txt)
{
    // For our dummy Problem, we use:
    //  - an empty URL,`
    //  - an empty vector of Clause,
    //  - and a dummy Construction.
    // The C++ constructor expects:
    //     Problem(const std::string &url, const std::vector<Clause> &clauses, const Construction &goal)
    // We assume that Construction has a constructor like:
    //     Construction(const std::string &name, const std::vector<std::string> &args)
    // If an empty vector isn’t accepted, we supply a dummy argument.
    std::vector<alphageometry::Clause> empty_clauses;
    std::vector<std::string> dummy_args = {"dummy_arg"};
    alphageometry::Construction dummy_goal("dummy_goal", dummy_args);
    return new alphageometry::Problem("", empty_clauses, dummy_goal);
}

alphageometry::Definition *create_dummy_definition(const std::string &name)
{
    // Create dummy Clause with empty vectors (since Clause has no default constructor).
    std::vector<std::string> dummy_pts;
    std::vector<alphageometry::Construction> dummy_cons;
    alphageometry::Clause dummy_clause(dummy_pts, dummy_cons);
    // Create a dummy Construction for the definition using the name and a dummy argument.
    std::vector<std::string> args = {"dummy_arg"};
    alphageometry::Construction cons(name, args);
    std::unordered_map<std::string, std::string> rely; // empty map
    std::vector<std::pair<std::vector<std::string>, std::vector<alphageometry::Construction>>> basics;
    std::vector<alphageometry::Construction> numerics;
    return new alphageometry::Definition(cons, rely, dummy_clause, basics, numerics);
}

namespace py = pybind11;

PYBIND11_MODULE(graph_cpp, m)
{
    m.doc() = "C++ port of graph.py for alphageometry (minimal DDAr mode)";

    py::class_<alphageometry::Problem>(m, "CppProblem")
        .def_static("from_txt", &create_problem_from_txt, "Create a Problem from text")
        .def_readwrite("goal", &alphageometry::Problem::goal);

    py::class_<alphageometry::DDAR>(m, "DDAR")
        .def_static("solve", &alphageometry::DDAR::solve,
                    py::arg("g"),
                    py::arg("defs"),
                    py::arg("p"),
                    py::arg("max_level"),
                    py::arg("timeout"));

    py::class_<alphageometry::Definition>(m, "Definition")
        .def_static("dummy", &create_dummy_definition, "Create a dummy Definition", py::arg("name"));

    py::class_<alphageometry::Graph>(m, "Graph")
        .def(py::init<>())
        .def("build_problem", &alphageometry::Graph::build_problem)
        .def("add_piece", &alphageometry::Graph::add_piece)
        .def("check", &alphageometry::Graph::check)
        .def("connect", &alphageometry::Graph::connect)
        .def("merge", &alphageometry::Graph::merge)
        .def("all_points", &alphageometry::Graph::all_points)
        .def("names2nodes", &alphageometry::Graph::names2nodes);

    py::class_<alphageometry::Construction>(m, "Construction")
        .def_readonly("name", &alphageometry::Construction::name)
        .def_readonly("args", &alphageometry::Construction::args);

    py::class_<alphageometry::Node>(m, "Node")
        .def_readonly("name", &alphageometry::Node::name);

    py::class_<alphageometry::Point, alphageometry::Node>(m, "Point")
        .def(py::init<const std::string &, double, double>())
        .def("distance", &alphageometry::Point::distance)
        .def("to_string", &alphageometry::Point::to_string);

    py::class_<alphageometry::Line, alphageometry::Node>(m, "Line")
        .def(py::init<alphageometry::Point *, alphageometry::Point *>())
        .def("is_on", &alphageometry::Line::is_on)
        .def("is_parallel", &alphageometry::Line::is_parallel)
        .def("is_perp", &alphageometry::Line::is_perp)
        .def("intersection", &alphageometry::Line::intersection)
        .def("perpendicular_line", &alphageometry::Line::perpendicular_line);

    py::class_<alphageometry::Circle, alphageometry::Node>(m, "Circle")
        .def(py::init<alphageometry::Point *, double>())
        .def(py::init<alphageometry::Point *, alphageometry::Point *, alphageometry::Point *>())
        .def_readonly("center", &alphageometry::Circle::center)
        .def_readonly("radius", &alphageometry::Circle::radius);

    py::class_<alphageometry::Segment, alphageometry::Node>(m, "Segment")
        .def(py::init<alphageometry::Point *, alphageometry::Point *>())
        .def("length", [](const alphageometry::Segment *s)
             { return s->endpoints[0]->distance(*s->endpoints[1]); });

    py::class_<alphageometry::Dependency>(m, "Dependency")
        .def(py::init<const std::string &, const std::vector<alphageometry::Point *> &, const std::string &, int>())
        .def("hashed", &alphageometry::Dependency::hashed);
}
