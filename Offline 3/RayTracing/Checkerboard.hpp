#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include "Object.hpp"

class Checkerboard : public Object {
public:
    double cellWidth;
    int windowWidth;
    int windowHeight;

    Checkerboard(){
        this->coEfficients[2] = 0; // no specular reflection
    }

    virtual void drawBoard(int windowWidth, int windowHeight){
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        draw();
    }
    
    virtual void draw(){
        //! draw the checkerboard infinitely with respect to the eye position
        for (int i = -windowWidth / cellWidth; i < windowWidth / cellWidth; ++i) {
            for (int j = -windowHeight / cellWidth; j < windowHeight / cellWidth; ++j) {
                float x = i * cellWidth;
                float y = j * cellWidth;

                if ((i + j) % 2) {
                    glColor3f(0.0f, 0.0f, 0.0f); // Black
                } else {
                    glColor3f(1.0f, 1.0f, 1.0f); // White
                }

                glBegin(GL_QUADS);
                    glVertex3f(x, y, 0);
                    glVertex3f(x + cellWidth, y, 0);
                    glVertex3f(x + cellWidth, y + cellWidth, 0);
                    glVertex3f(x, y + cellWidth, 0);
                glEnd();
            }
        }
    }
};

#endif // CHECKERBOARD_HPP