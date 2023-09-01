#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

#include "Point.hpp"

class LightSource
{
public:
    Point position;
    double falloff;
    int type; // 0 = normal, 1 = spot

    virtual void draw(){
        // draw the light source
        // a sphere of radius 5
        Color color = Color(0.5, 0.5, 0.5);
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere(5, 100, 100);
        glPopMatrix();
    }
};

#endif // LIGHTSOURCE_HPP