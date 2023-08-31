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
        this->height = height;
    }

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
        // 5 points
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

        if (t1 > 0){
            ray->setIntersectionPoint(ray->start + ray->direction * t1);
            return t1;
        } else if (t2 > 0){
            ray->setIntersectionPoint(ray->start + ray->direction * t2);
            return t2;
        } else if (t3 > 0){
            ray->setIntersectionPoint(ray->start + ray->direction * t3);
            return t3;
        } else if (t4 > 0){
            ray->setIntersectionPoint(ray->start + ray->direction * t4);
            return t4;
        } else if (t5 > 0){
            ray->setIntersectionPoint(ray->start + ray->direction * t5);
            return t5;
        } else {
            return -1;
        }
    }
};



#endif // PYRAMID_HPP