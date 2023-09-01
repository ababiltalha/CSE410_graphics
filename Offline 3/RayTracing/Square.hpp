#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "Object.hpp"

class Square
{
public:
    Point lowerLeft, upperRight;

    Square(Point lowerLeft, Point upperRight){
        this->lowerLeft = lowerLeft;
        this->upperRight = upperRight;
    }

    virtual double intersect(Ray* ray){
        // cases: square parallel to XY
        if (lowerLeft.z == upperRight.z){
            // ray parallel to XY
            if (ray->direction.z == 0) return -1;
            double t = (lowerLeft.z - ray->start.z) / ray->direction.z;
            Point intersectionPoint = ray->start + ray->direction * t;
            if (intersectionPoint.x >= lowerLeft.x && intersectionPoint.x <= upperRight.x && intersectionPoint.y >= lowerLeft.y && intersectionPoint.y <= upperRight.y){
                ray->setIntersectionPoint(intersectionPoint);
                return t;
            }
        }

        // cases: square parallel to YZ
        if (lowerLeft.x == upperRight.x){
            // ray parallel to YZ
            if (ray->direction.x == 0) return -1;
            double t = (lowerLeft.x - ray->start.x) / ray->direction.x;
            Point intersectionPoint = ray->start + ray->direction * t;
            if (intersectionPoint.z >= lowerLeft.z && intersectionPoint.z <= upperRight.z && intersectionPoint.y >= lowerLeft.y && intersectionPoint.y <= upperRight.y){
                ray->setIntersectionPoint(intersectionPoint);
                return t;
            }
        }

        // cases: square parallel to XZ
        if (lowerLeft.y == upperRight.y){
            // ray parallel to XZ
            if (ray->direction.y == 0) return -1;
            double t = (lowerLeft.y - ray->start.y) / ray->direction.y;
            Point intersectionPoint = ray->start + ray->direction * t;
            if (intersectionPoint.x >= lowerLeft.x && intersectionPoint.x <= upperRight.x && intersectionPoint.z >= lowerLeft.z && intersectionPoint.z <= upperRight.z){
                ray->setIntersectionPoint(intersectionPoint);
                return t;
            }
        }
        return -1;
    }



};

#endif // SQUARE_HPP