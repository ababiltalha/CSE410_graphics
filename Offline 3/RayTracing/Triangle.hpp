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

class Triangle
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
        } else return -1;
    }

    virtual Point normalAt(Point point, Ray* ray){
        // find normal at point on triangle surface
        Point normal = (b - a).cross(c - a);
        normal.normalize();

        if (normal.dot(ray->direction) > EPSILON){
            normal = normal * -1;
        }
        // std::ofstream fout("debug.txt", std::ios::app);
        // fout << "normal: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
        return normal;
    }

    bool isInside(Point point){
        Point ca = c - a;
        Point ba = b - a;
        Point pa = point - a;

        double dot00 = ca.dot(ca);
        double dot01 = ca.dot(ba);
        double dot02 = ca.dot(pa);
        double dot11 = ba.dot(ba);
        double dot12 = ba.dot(pa);

        double denom = dot00 * dot11 - dot01 * dot01;

        double u = (dot11 * dot02 - dot01 * dot12) / denom;
        double v = (dot00 * dot12 - dot01 * dot02) / denom;

        return (u >= 0) && (v >= 0) && (u + v <= 1);
    }




};

#endif // TRIANGLE_HPP