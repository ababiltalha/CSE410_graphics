#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "Object.hpp"

class Pyramid : public Object
{
public:
    Point lowest;
    double width, height;
};

#endif // PYRAMID_HPP