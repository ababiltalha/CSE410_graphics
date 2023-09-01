#ifndef RAY_HPP
#define RAY_HPP

#include "Point.hpp"

class Ray {
public:
    Point start;
    Point direction;
    Point intersectionPoint;

    Ray(Point start, Point dir){
        this->start = start;
        this->direction = dir;
        this->direction.normalize();
    }

    void setStart(Point start){
        this->start = start;
    }
    
    void setDirection(Point dir){
        this->direction = dir;
        dir.normalize();
    }

    void setIntersectionPoint(Point intersectionPoint){
        this->intersectionPoint = intersectionPoint;
    }
};

#endif // RAY_HPP