#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "Object.hpp"

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
        glColor3f(color[0], color[1], color[2]);
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
};



#endif // PYRAMID_HPP