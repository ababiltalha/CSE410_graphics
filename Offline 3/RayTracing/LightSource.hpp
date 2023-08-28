#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

#include "Point.hpp"

class LightSource
{
public:
    Point position;
    double falloff;
};

#endif // LIGHTSOURCE_HPP