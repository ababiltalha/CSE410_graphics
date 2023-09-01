#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include "bitmap_image.hpp"

bitmap_image black("texture_b.bmp");
bitmap_image white("texture_w.bmp");

#include "Object.hpp"

class Checkerboard : public Object {
public:
    double cellWidth;
    int windowWidth;
    int windowHeight;
    bool showTexture;

    Checkerboard(){
        this->coEfficients[SPECULAR] = 0; // no specular reflection
        this->showTexture = false;
        
    }

    void drawBoard(int windowWidth, int windowHeight){
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        draw();
    }

    void toggleTexture(){
        showTexture = !showTexture;
    }

    bool isTextureOn(){
        return showTexture;
    }
    
    void draw(){
        int widthRange = floor(windowWidth / cellWidth);
        int heightRange = floor(windowHeight / cellWidth);
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
        // ray->setIntersectionPoint(ray->start + ray->direction * t);
        if (t < 0) return -1;
        return t;
    }

    virtual Color getColor(Point intersectionPoint){
        int i = floor(intersectionPoint.x / cellWidth);
        int j = floor(intersectionPoint.y / cellWidth);

        if ((i + j) % 2) {
            if (showTexture) {
                int textureX = ((intersectionPoint.x / cellWidth) - floor(intersectionPoint.x / cellWidth)) * (black.width() -1);
                int textureY = ((intersectionPoint.y / cellWidth) - floor(intersectionPoint.y / cellWidth)) * (black.height() -1);

                unsigned char r, g, b;
                black.get_pixel(textureX, textureY, r, g, b);
                return Color(r / 255.0, g / 255.0, b / 255.0);
            } else 
                return Color(0, 0, 0);
        } else {
            if (showTexture) {
                int textureX = ((intersectionPoint.x / cellWidth) - floor(intersectionPoint.x / cellWidth)) * (white.width() -1);
                int textureY = ((intersectionPoint.y / cellWidth) - floor(intersectionPoint.y / cellWidth)) * (white.height() -1);

                unsigned char r, g, b;
                white.get_pixel(textureX, textureY, r, g, b);
                return Color(r / 255.0, g / 255.0, b / 255.0);
            }
            else 
                return Color(1, 1, 1);
        }
    }

    virtual Point normalAt(Point point, Ray* ray){
        if (abs(point.z) > EPSILON) cout << "Error: point.z != 0" << endl;
        if (ray->direction.z > 0) return Point(0, 0, -1);
        return Point(0, 0, 1);
    }

};

#endif // CHECKERBOARD_HPP