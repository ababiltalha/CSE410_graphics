#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

#include "Point.hpp"

class LightSource
{
public:
    Point position;
    double falloff;
    int type; // 0 = normal, 1 = spot

    virtual void draw() = 0;
};

#endif // LIGHTSOURCE_HPP