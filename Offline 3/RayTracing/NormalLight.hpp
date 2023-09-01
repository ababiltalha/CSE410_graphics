#ifndef NORMAL_LIGHT_HPP
#define NORMAL_LIGHT_HPP

#include "LightSource.hpp"

class NormalLight : public LightSource
{
public:
    NormalLight(){
        type = 0;
    }

    void draw(){
        // draw the light source
        // a sphere of radius 5
        Color color = Color(0.5, 0.1, 0.1);
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere(2, 100, 100);
        glPopMatrix();
    }
};

#endif // NORMAL_LIGHT_HPP