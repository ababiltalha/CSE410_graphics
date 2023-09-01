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

    void setDirection(Point lookAt){
        direction = lookAt - position;
        direction.normalize();
    }

    void draw(){
        // draw the light source
        // a sphere of radius 5
        Color color = Color(0.1, 0.5, 0.1);
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere(2, 100, 100);
        glPopMatrix();
    }
};

#endif // SPOT_LIGHT_HPP