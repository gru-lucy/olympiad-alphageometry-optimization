// geometry.cpp
#include "geometry.hpp"
#include <stdexcept>
#include <sstream>

namespace alphageometry
{

    Point line_line_intersection(const Line &l1, const Line &l2)
    {
        double det = l1.a * l2.b - l2.a * l1.b;
        if (std::fabs(det) < 1e-6)
        {
            throw std::runtime_error("Lines do not intersect (parallel).");
        }
        double x = (l2.b * (-l1.c) - l1.b * (-l2.c)) / det;
        double y = (l1.a * (-l2.c) - l2.a * (-l1.c)) / det;
        return Point("intersection", x, y);
    }

    Point Line::intersection(const Line &other) const
    {
        return line_line_intersection(*this, other);
    }

    Line Line::perpendicular_line(Point *p) const
    {
        // The perpendicular line to this line through point p.
        // If current line has coefficients (a, b, c), the perpendicular line has (b, -a, c')
        double new_a = b;
        double new_b = -a;
        // double new_c = -(new_a * p->x + new_b * p->y);
        // We create a new Line using p and a second point computed by offsetting p.
        Point *p2 = new Point("temp", p->x + new_b, p->y - new_a);
        Line pl(p, p2);
        return pl;
    }

    Circle::Circle(Point *p1, Point *p2, Point *p3)
        : Node("Circle_" + p1->name + p2->name + p3->name)
    {
        // Compute perpendicular bisectors of p1p2 and p2p3, then compute the center and radius.
        Point mid1("mid1", 0.5 * (p1->x + p2->x), 0.5 * (p1->y + p2->y));
        Point mid2("mid2", 0.5 * (p2->x + p3->x), 0.5 * (p2->y + p3->y));
        Line l1(p1, p2);
        Line l2(p2, p3);
        Line pb1 = l1.perpendicular_line(&mid1);
        Line pb2 = l2.perpendicular_line(&mid2);
        Point o = pb1.intersection(pb2);
        center = new Point("O", o.x, o.y);
        radius = center->distance(*p1);
    }

    std::string Circle::to_string() const
    {
        std::ostringstream oss;
        oss << "Circle(center=" << center->to_string() << ", radius=" << radius << ")";
        return oss.str();
    }

} // namespace alphageometry
