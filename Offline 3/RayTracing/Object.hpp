#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Point.hpp"
#include "Color.hpp"
#include "Ray.hpp"

class Object {
public:
    Color color; // RGB
    double coEfficients[4]; // ambient, diffuse, specular, reflection
    int shininess; // exponent term of specular equation

    void setColor(Color color){
        this->color = color;
    }
    void setCoEfficients(double coEfficients[]){
        for (int i = 0; i < 4; ++i) {
            this->coEfficients[i] = coEfficients[i];
        }
    }
    void setShininess(int shininess){
        this->shininess = shininess;
    }

    virtual void draw(){
        // draw the object
    }

    virtual double intersect(Ray ray){
        // return the distance from the source to the object
        return 0;
    }

    virtual Color getColor(Point intersectionPoint){
        // return the color of the object at the intersection point
        return color;
    }
};

#endif // OBJECT_HPP