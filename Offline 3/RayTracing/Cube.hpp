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
        glColor3f(color[0], color[1], color[2]);
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
};

#endif // CUBE_HPP