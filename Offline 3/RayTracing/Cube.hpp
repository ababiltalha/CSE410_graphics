#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object.hpp"

class Cube : public Object
{
public:
    Point bottomLowerLeft;
    double edge;

    Cube(Point bottomLowerLeft, double edge){
        this->bottomLowerLeft = bottomLowerLeft;
        this->edge = edge;
    }

    virtual void draw(){
        // draw the cube with respect to bottom lower left point and edge length
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(bottomLowerLeft.x, bottomLowerLeft.y, bottomLowerLeft.z);
            glScalef(edge, edge, edge);
            drawCube();
        glPopMatrix();
    }

    void drawCube(){
        glBegin(GL_QUADS);
            // Front face
            glVertex3f(0, 0, 0);
            glVertex3f(1, 0, 0);
            glVertex3f(1, 0, 1);
            glVertex3f(0, 0, 1);

            // Back face
            glVertex3f(0, 1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(1, 1, 1);
            glVertex3f(0, 1, 1);

            // Left face
            glVertex3f(0, 0, 0);
            glVertex3f(0, 1, 0);
            glVertex3f(0, 1, 1);
            glVertex3f(0, 0, 1);

            // Right face
            glVertex3f(1, 0, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(1, 1, 1);
            glVertex3f(1, 0, 1);

            // Top face
            glVertex3f(0, 0, 1);
            glVertex3f(1, 0, 1);
            glVertex3f(1, 1, 1);
            glVertex3f(0, 1, 1);

            // Bottom face
            glVertex3f(0, 0, 0);
            glVertex3f(1, 0, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(0, 1, 0);      
        glEnd();
    }

    virtual double intersect(Ray ray){
        // find intersection of the ray with the cube
        // if intersected then return distance from source to intersection point
        // otherwise return -1
        return -1;
    }
};

#endif // CUBE_HPP