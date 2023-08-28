#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object.hpp"

class Cube : public Object
{
public:
    Point bottomLowerLeft;
    double edge;
};

#endif // CUBE_HPP