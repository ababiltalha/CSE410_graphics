#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "LightSource.hpp"

class SpotLight : public LightSource
{
public:
    Point direction;
    double cutoffAngle;

    SpotLight(){
        type = 1;
    }
};

#endif // SPOT_LIGHT_HPP