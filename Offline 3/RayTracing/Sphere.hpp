#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object.hpp"

class Sphere : public Object {
public:
    Point center;
    double radius;
};

#endif // SPHERE_HPP