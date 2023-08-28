#ifndef POINT_HPP
#define POINT_HPP

class Point {
public:
    double x, y, z;

    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    // Point(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
    Point() : x(0), y(0), z(0) {}

    Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y, z + p.z);
    }

    Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y, z - p.z);
    }

    Point operator*(double d) const {
        return Point(x * d, y * d, z * d);
    }

    Point operator/(double d) const {
        return Point(x / d, y / d, z / d);
    }

    double dot(const Point &p) const {
        return (x * p.x + y * p.y + z * p.z);
    }

    Point cross(const Point &p) const {
        return Point(y * p.z - z * p.y,
                    z * p.x - x * p.z,
                    x * p.y - y * p.x);
    }

    void normalize() {
        double d = sqrt(x * x + y * y + z * z);
        x /= d;
        y /= d;
        z /= d;
    }
};

#endif // POINT_HPP