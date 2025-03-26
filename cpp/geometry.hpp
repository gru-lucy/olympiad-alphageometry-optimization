// geometry.hpp
#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
#include <string>
#include <sstream>
#include "node.hpp" // Include the Node definition

namespace alphageometry
{

    // Point now inherits from Node.
    class Point : public Node
    {
    public:
        double x, y;

        Point(const std::string &name, double x, double y)
            : Node(name), x(x), y(y) {}

        double distance(const Point &other) const
        {
            double dx = x - other.x;
            double dy = y - other.y;
            return std::sqrt(dx * dx + dy * dy);
        }

        std::string to_string() const override
        {
            std::ostringstream oss;
            oss << "Point(" << x << ", " << y << ")";
            return oss.str();
        }
    };

    // Similarly, update Line and Circle so that they inherit from Node.
    // For example, here’s a minimal change for Line:
    class Line : public Node
    {
    public:
        // Coefficients for ax + by + c = 0.
        double a, b, c;
        Point *p1;
        Point *p2;

        Line(Point *p1, Point *p2)
            : Node("Line_" + p1->name + "_" + p2->name), p1(p1), p2(p2)
        {
            a = p1->y - p2->y;
            b = p2->x - p1->x;
            c = p1->x * p2->y - p2->x * p1->y;
            if (a < 0 || (std::fabs(a) < 1e-12 && b > 0))
            {
                a = -a;
                b = -b;
                c = -c;
            }
        }

        double eval(double x, double y) const
        {
            return a * x + b * y + c;
        }

        bool is_on(const Point &p) const
        {
            return std::fabs(eval(p.x, p.y)) < 1e-6;
        }

        bool is_parallel(const Line &other) const
        {
            return std::fabs(a * other.b - other.a * b) < 1e-6;
        }

        bool is_perp(const Line &other) const
        {
            return std::fabs(a * other.a + b * other.b) < 1e-6;
        }

        // Provide an implementation for the pure virtual function.
        std::string to_string() const override
        {
            std::ostringstream oss;
            oss << "Line(" << p1->to_string() << ", " << p2->to_string() << ")";
            return oss.str();
        }

        // Declarations for intersection and perpendicular_line remain unchanged.
        Point intersection(const Line &other) const;
        Line perpendicular_line(Point *p) const;
    };

    // And update Circle similarly:
    class Circle : public Node
    {
    public:
        Point *center;
        double radius;

        // Construct with given center and radius.
        Circle(Point *center, double radius)
            : Node("Circle_" + center->name), center(center), radius(radius) {}

        // Construct the circle from three points.
        Circle(Point *p1, Point *p2, Point *p3);

        std::string to_string() const override;
    };

    class Segment : public Node
    {
    public:
        // Store the two endpoints.
        Point *endpoints[2];

        Segment(Point *a, Point *b)
            : Node("Segment_" + a->name + "_" + b->name)
        {
            endpoints[0] = a;
            endpoints[1] = b;
        }

        std::string to_string() const override
        {
            return "Segment(" + endpoints[0]->to_string() + ", " + endpoints[1]->to_string() + ")";
        }
    };

} // namespace alphageometry

#endif // GEOMETRY_HPP
