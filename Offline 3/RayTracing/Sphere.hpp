#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object.hpp"

class Sphere : public Object {
public:
    Point center;
    double radius;

    Sphere(Point center, double radius){
        this->center = center;
        this->radius = radius;
    }

    virtual void draw(){
        // draw the sphere with respect to center and radius
        // std::cout << "Drawing sphere" << std::endl;
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glTranslatef(center.x, center.y, center.z);
            glutSolidSphere(radius, 100, 100);
        glPopMatrix();
    }
};

#endif // SPHERE_HPP