#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Point.hpp"

class Object {
public:
    double color[3]; // RGB
    double coEfficients[4]; // ambient, diffuse, specular, reflection
    int shininess; // exponent term of specular equation

    void setColor(double color[]){
        for (int i = 0; i < 3; ++i) {
            this->color[i] = color[i];
        }
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
};

#endif // OBJECT_HPP