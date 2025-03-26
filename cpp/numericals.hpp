// numericals.hpp
#ifndef NUMERICALS_HPP
#define NUMERICALS_HPP

#include "geometry.hpp"
#include <cmath>
#include <vector>

namespace alphageometry
{

    static const double ATOM = 1e-12;

    // Minimal implementations of numerical functions

    inline bool close_enough(double a, double b, double tol = 1e-6)
    {
        return std::fabs(a - b) < tol;
    }

    inline std::vector<alphageometry::Point *> random_points(int n)
    {
        std::vector<alphageometry::Point *> pts;
        for (int i = 0; i < n; i++)
        {
            double x = ((double)rand() / RAND_MAX) * 2 - 1;
            double y = ((double)rand() / RAND_MAX) * 2 - 1;
            pts.push_back(new alphageometry::Point("rp" + std::to_string(i), x, y));
        }
        return pts;
    }

    // Placeholder for additional numerical functions (e.g. angle between, etc.)
    inline double ang_between(const alphageometry::Point &tail,
                              const alphageometry::Point &p1,
                              const alphageometry::Point &p2)
    {
        double dx1 = p1.x - tail.x, dy1 = p1.y - tail.y;
        double dx2 = p2.x - tail.x, dy2 = p2.y - tail.y;
        double angle1 = std::atan2(dy1, dx1);
        double angle2 = std::atan2(dy2, dx2);
        double diff = angle1 - angle2;
        while (diff > M_PI)
            diff -= 2 * M_PI;
        while (diff < -M_PI)
            diff += 2 * M_PI;
        return diff;
    }

} // namespace alphageometry

#endif // NUMERICALS_HPP
