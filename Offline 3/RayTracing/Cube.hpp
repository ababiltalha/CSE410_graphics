#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object.hpp"
#include "Square.hpp"

class Cube : public Object
{
public:
    Point bottomLowerLeft;
    double edge;

    Cube(Point bottomLowerLeft, double edge){
        this->bottomLowerLeft = bottomLowerLeft;
        this->edge = edge;
    }

    void draw(){
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

    double intersect(Ray* ray){
        // Bottom face
        Square sq1(bottomLowerLeft, bottomLowerLeft + Point(edge, edge, 0));
        // Top face
        Square sq2(bottomLowerLeft + Point(0, 0, edge), bottomLowerLeft + Point(edge, edge, edge));
        // Left face
        Square sq3(bottomLowerLeft, bottomLowerLeft + Point(0, edge, edge));
        // Right face
        Square sq4(bottomLowerLeft + Point(edge, 0, 0), bottomLowerLeft + Point(edge, edge, edge));
        // Front face
        Square sq5(bottomLowerLeft, bottomLowerLeft + Point(edge, 0, edge));
        // Back face
        Square sq6(bottomLowerLeft + Point(0, edge, 0), bottomLowerLeft + Point(edge, edge, edge));
        
        double t1 = sq1.intersect(ray);
        double t2 = sq2.intersect(ray);
        double t3 = sq3.intersect(ray);
        double t4 = sq4.intersect(ray);
        double t5 = sq5.intersect(ray);
        double t6 = sq6.intersect(ray);

        double minT = t1;
        if (t2 > 0 && (minT < 0 || t2 < minT)) minT = t2;
        if (t3 > 0 && (minT < 0 || t3 < minT)) minT = t3;
        if (t4 > 0 && (minT < 0 || t4 < minT)) minT = t4;
        if (t5 > 0 && (minT < 0 || t5 < minT)) minT = t5;
        if (t6 > 0 && (minT < 0 || t6 < minT)) minT = t6;

        if (minT > 0){
            // ray->setIntersectionPoint(ray->start + ray->direction * minT);
            return minT;
        } else {
            return -1;
        }
    }

    Point normalAt(Point point, Ray* ray){
        // find normal at point on cube surface
        // Bottom face
        if (abs(point.z - bottomLowerLeft.z) < EPSILON){
            if (ray->direction.z > 0) return Point(0, 0, -1);
            else return Point(0, 0, 1);
        }
        // Top face
        if (abs(point.z - bottomLowerLeft.z - edge) < EPSILON){
            if (ray->direction.z > 0) return Point(0, 0, -1);
            else return Point(0, 0, 1);
        }
        // Left face
        if (abs(point.x - bottomLowerLeft.x) < EPSILON){
            if (ray->direction.x > 0) return Point(-1, 0, 0);
            else return Point(1, 0, 0);
        }
        if (abs(point.x - bottomLowerLeft.x - edge) < EPSILON){
            if (ray->direction.x > 0) return Point(-1, 0, 0);
            else return Point(1, 0, 0);
        }
        if (abs(point.y - bottomLowerLeft.y) < EPSILON){
            if (ray->direction.y > 0) return Point(0, -1, 0);
            else return Point(0, 1, 0);
        }
        if (abs(point.y - bottomLowerLeft.y - edge) < EPSILON){
            if (ray->direction.y > 0) return Point(0, -1, 0);
            else return Point(0, 1, 0);
        }
        std::cout << "Error: Point not on cube surface" << std::endl;
        std::cout << "Cube: " << bottomLowerLeft.x << " " << bottomLowerLeft.y << " " << bottomLowerLeft.z << " " << edge << std::endl;
        std::cout << "Point: " << point.x << " " << point.y << " " << point.z << std::endl;
        return Point(0, 0, 0);
    }
};

#endif // CUBE_HPP