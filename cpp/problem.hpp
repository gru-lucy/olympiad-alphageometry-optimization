// problem.hpp
#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

namespace alphageometry
{

    class Construction
    {
    public:
        std::string name;
        std::vector<std::string> args;

        Construction(const std::string &name, const std::vector<std::string> &args)
            : name(name), args(args) {}

        std::string txt() const
        {
            std::ostringstream oss;
            oss << name;
            for (const auto &arg : args)
            {
                oss << " " << arg;
            }
            return oss.str();
        }
    };

    class Clause
    {
    public:
        std::vector<std::string> points;
        std::vector<Construction> constructions;

        Clause(const std::vector<std::string> &pts,
               const std::vector<Construction> &cons)
            : points(pts), constructions(cons) {}

        std::string txt() const
        {
            std::ostringstream oss;
            for (size_t i = 0; i < points.size(); i++)
            {
                oss << points[i] << " ";
            }
            oss << "= ";
            for (size_t i = 0; i < constructions.size(); i++)
            {
                oss << constructions[i].txt();
                if (i != constructions.size() - 1)
                    oss << ", ";
            }
            return oss.str();
        }
    };

    class Problem
    {
    public:
        std::string url;
        std::vector<Clause> clauses;
        Construction goal; // assume only one goal; if none, goal.name == ""

        Problem(const std::string &url,
                const std::vector<Clause> &clauses,
                const Construction &goal)
            : url(url), clauses(clauses), goal(goal) {}

        Problem translate() const;
        std::string txt() const;
    };

    class Definition
    {
    public:
        Construction construction;
        std::unordered_map<std::string, std::string> rely;
        Clause deps;
        // For simplicity, basics and numerics are stored as constructions.
        std::vector<std::pair<std::vector<std::string>, std::vector<Construction>>> basics;
        std::vector<Construction> numerics;

        Definition(const Construction &cons,
                   const std::unordered_map<std::string, std::string> &rely,
                   const Clause &deps,
                   const std::vector<std::pair<std::vector<std::string>, std::vector<Construction>>> &basics,
                   const std::vector<Construction> &numerics)
            : construction(cons), rely(rely), deps(deps), basics(basics), numerics(numerics) {}

        static Definition from_txt(const std::string &data);
    };

} // namespace alphageometry

#endif // PROBLEM_HPP
