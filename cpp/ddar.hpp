// ddar.hpp
#ifndef DDAR_HPP
#define DDAR_HPP

#include "graph.hpp"
#include "problem.hpp"
#include <vector>

namespace alphageometry
{

    // DDAr: combination of DD (deduction derivations) and AR (algebraic reasoning).
    // Minimal implementation to support DDAr tests.

    class DDAR
    {
    public:
        // Run DD until saturation or goal is found.
        // Returns a list of derivations (dependencies).
        static std::vector<Dependency> saturate_or_goal(Graph &g,
                                                        const std::unordered_map<std::string, Definition> &theorems,
                                                        const Problem &p,
                                                        int max_level = 100);

        // Alternate between DD and AR until goal is found.
        static std::tuple<Graph, std::vector<double>, std::string, std::vector<int>, std::vector<Dependency>>
        solve(Graph &g,
              const std::unordered_map<std::string, Definition> &theorems,
              const Problem &p,
              int max_level = 100,
              int timeout = 600);
    };

} // namespace alphageometry

#endif // DDAR_HPP
