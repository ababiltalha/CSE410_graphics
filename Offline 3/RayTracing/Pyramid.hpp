#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "Object.hpp"
#include "Triangle.hpp"
#include "Square.hpp"

class Pyramid : public Object
{
public:
    Point lowest;
    double width, height;

    Pyramid(Point lowest, double width, double height){
        this->lowest = lowest;
        this->width = width;
        this->height = height;    }

    virtual void draw(){
        // draw the pyramid with respect to lowest point, width and height
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(lowest.x + width/2, lowest.y + width/2, lowest.z);
            glScalef(width/2, width/2, height);
            drawPyramid();
        glPopMatrix();
    }

    void drawPyramid(){
        glBegin(GL_TRIANGLES);
            // Front face
            glVertex3f(0, 0, 1);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);

            // Right face
            glVertex3f(0, 0, 1);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);

            // Back face
            glVertex3f(0, 0, 1);
            glVertex3f(1, 1, 0);
            glVertex3f(-1, 1, 0);

            // Left face
            glVertex3f(0, 0, 1);
            glVertex3f(-1, 1, 0);
            glVertex3f(-1, -1, 0);
        glEnd();

        glBegin(GL_QUADS);
            // Bottom face
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(-1, 1, 0);
        glEnd();
    }

    virtual double intersect(Ray* ray){
        
        Point a = lowest;
        Point b = lowest + Point(width, 0, 0);
        Point c = lowest + Point(width, width, 0);
        Point d = lowest + Point(0, width, 0);
        Point e = lowest + Point(width/2, width/2, height);

        Triangle tri1(a, b, e);
        Triangle tri2(b, c, e);
        Triangle tri3(c, d, e);
        Triangle tri4(d, a, e);
        Square sq(a, c);


        double t1 = tri1.intersect(ray);
        double t2 = tri2.intersect(ray);
        double t3 = tri3.intersect(ray);
        double t4 = tri4.intersect(ray);
        double t5 = sq.intersect(ray);

        double minT = t1;
        if (t2 > 0 && (minT < 0 || t2 < minT)) minT = t2;
        if (t3 > 0 && (minT < 0 || t3 < minT)) minT = t3;
        if (t4 > 0 && (minT < 0 || t4 < minT)) minT = t4;
        if (t5 > 0 && (minT < 0 || t5 < minT)) minT = t5;

        if (minT > 0){
            // ray->setIntersectionPoint(ray->start + ray->direction * minT);
            return minT;
        } else {
            return -1;
        }

        // if (t1 > 0){
        //     // ray->setIntersectionPoint(ray->start + ray->direction * t1);
        //     return t1;
        // } else if (t2 > 0){
        //     // ray->setIntersectionPoint(ray->start + ray->direction * t2);
        //     return t2;
        // } else if (t3 > 0){
        //     // ray->setIntersectionPoint(ray->start + ray->direction * t3);
        //     return t3;
        // } else if (t4 > 0){
        //     // ray->setIntersectionPoint(ray->start + ray->direction * t4);
        //     return t4;
        // } else if (t5 > 0){
        //     // ray->setIntersectionPoint(ray->start + ray->direction * t5);
        //     return t5;
        // } else {
        //     return -1;
        // }
    }

    virtual Point normalAt(Point point, Ray* ray){
        // Bottom face
        if (abs(point.z - lowest.z) < EPSILON){
            if (ray->direction.z > 0) return Point(0, 0, -1);
            else return Point(0, 0, 1);
        }

        // Triangles
        Point a = lowest;
        Point b = lowest + Point(width, 0, 0);
        Point c = lowest + Point(width, width, 0);
        Point d = lowest + Point(0, width, 0);
        Point e = lowest + Point(width/2, width/2, height);

        Triangle tri1(a, b, e);
        Triangle tri2(b, c, e);
        Triangle tri3(c, d, e);
        Triangle tri4(d, a, e);

        // double t1 = tri1.intersect(ray);
        // double t2 = tri2.intersect(ray);
        // double t3 = tri3.intersect(ray);
        // double t4 = tri4.intersect(ray);

        // Point tri1Point = ray->start + ray->direction * t1;
        // Point tri2Point = ray->start + ray->direction * t2;
        // Point tri3Point = ray->start + ray->direction * t3;
        // Point tri4Point = ray->start + ray->direction * t4;

        // if (abs(tri1Point.x - a.x) < EPSILON && abs(tri1Point.y - a.y) < EPSILON && abs(tri1Point.z - a.z) < EPSILON){
        //     return tri1.normalAt(point, ray);
        // } else if (abs(tri2Point.x - b.x) < EPSILON && abs(tri2Point.y - b.y) < EPSILON && abs(tri2Point.z - b.z) < EPSILON){
        //     return tri2.normalAt(point, ray);
        // } else if (abs(tri3Point.x - c.x) < EPSILON && abs(tri3Point.y - c.y) < EPSILON && abs(tri3Point.z - c.z) < EPSILON){
        //     return tri3.normalAt(point, ray);
        // } else if (abs(tri4Point.x - d.x) < EPSILON && abs(tri4Point.y - d.y) < EPSILON && abs(tri4Point.z - d.z) < EPSILON){
        //     return tri4.normalAt(point, ray);
        // }

        if (tri1.isInside(point)){
            return tri1.normalAt(point, ray);
        } else if (tri2.isInside(point)){
            return tri2.normalAt(point, ray);
        } else if (tri3.isInside(point)){
            return tri3.normalAt(point, ray);
        } else if (tri4.isInside(point)){
            return tri4.normalAt(point, ray);
        }

        std::cout << "Error: Pyramid normalAt" << std::endl;
        return Point(0, 0, 0);
    }
};



#endif // PYRAMID_HPP