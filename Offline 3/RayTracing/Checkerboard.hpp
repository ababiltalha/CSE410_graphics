#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include "Object.hpp"

class Checkerboard : public Object {
public:
    double cellWidth;

    virtual void draw(){
        // draw the checkerboard infinitely
        glPushMatrix();
            glTranslatef(-100, 0, -100);
            for(int i = 0; i < 200; i++){
                for(int j = 0; j < 200; j++){
                    if((i + j) % 2 == 0){
                        glColor3f(1, 1, 1);
                    } else {
                        glColor3f(0, 0, 0);
                    }
                    glBegin(GL_QUADS);
                        glVertex3f(i * cellWidth, 0, j * cellWidth);
                        glVertex3f((i + 1) * cellWidth, 0, j * cellWidth);
                        glVertex3f((i + 1) * cellWidth, 0, (j + 1) * cellWidth);
                        glVertex3f(i * cellWidth, 0, (j + 1) * cellWidth);
                    glEnd();
                }
            }
    }
};

#endif // CHECKERBOARD_HPP