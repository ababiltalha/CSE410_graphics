#ifndef RAY_HPP
#define RAY_HPP

#include "Point.hpp"

class Ray {
public:
    Point start;
    Point direction;

    Ray(Point start, Point dir){
        this->start = start;
        this->direction = dir;
        dir.normalize();
    }

    
};

#endif // RAY_HPP