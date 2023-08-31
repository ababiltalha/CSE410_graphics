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
        //? how to understand color of tile in intersect?
        int widthRange = ceil(windowWidth / cellWidth);
        int heightRange = ceil(windowHeight / cellWidth);
        for (int i = -widthRange; i < widthRange; ++i) {
            for (int j = -heightRange; j < heightRange; ++j) {
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

    virtual double intersect(Ray* ray){
        double t = (-ray->start.z) / ray->direction.z;
        ray->setIntersectionPoint(ray->start + ray->direction * t);
        if (t < 0) return -1;
        return t;
    }

    virtual Color getColor(Point intersectionPoint){
        int i = ceil(intersectionPoint.x / cellWidth);
        int j = ceil(intersectionPoint.y / cellWidth);
        if ((i + j) % 2) {
            return Color(0, 0, 0);
        } else {
            return Color(1, 1, 1);
        }
    }
};

#endif // CHECKERBOARD_HPP