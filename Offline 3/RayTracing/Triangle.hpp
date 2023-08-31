#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Object.hpp"

class Matrix {
public:
    double matrix[3][3];

    Matrix(){
        // initialize with identity matrix
    }

    double getDeterminant(){
        return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
                - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
                + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    }

    Matrix(Point col1, Point col2, Point col3){
        matrix[0][0] = col1.x;  matrix[0][1] = col2.x;  matrix[0][2] = col3.x;
        matrix[1][0] = col1.y;  matrix[1][1] = col2.y;  matrix[1][2] = col3.y;
        matrix[2][0] = col1.z;  matrix[2][1] = col2.z;  matrix[2][2] = col3.z;
    }

};

class Triangle : public Object
{
public:
    Point a, b, c;

    Triangle(Point a, Point b, Point c){
        this->a = a;
        this->b = b;
        this->c = c;
    }

    virtual double intersect(Ray* ray){
        // use the concept of barycentric coordinates
        Matrix matA(a - b, a - c, ray->direction);
        Matrix matBeta(a - ray->start, a - c, ray->direction);
        Matrix matGamma(a - b, a - ray->start, ray->direction);
        Matrix matT(a - b, a - c, a - ray->start);

        double detA = matA.getDeterminant();
        double detBeta = matBeta.getDeterminant() / detA;
        double detGamma = matGamma.getDeterminant() / detA;
        double detT = matT.getDeterminant() / detA;

        if (detBeta >= 0 && detGamma >= 0 && detBeta + detGamma <= 1 && detT > 0){
            // ray->setIntersectionPoint(ray->start + ray->direction * detT);
            return detT;
        } else {
            return -1;
        }
    }



};

#endif // TRIANGLE_HPP