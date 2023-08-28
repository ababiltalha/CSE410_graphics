#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Point.hpp"

class Object {
public:
    double color[3]; // RGB
    double coEfficients[4]; // ambient, diffuse, specular, reflection
    int shininess; // exponent term of specular equation
};

#endif // OBJECT_HPP