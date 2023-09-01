#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object.hpp"
#include "Point.hpp"

class Sphere : public Object {
public:
    Point center;
    double radius;

    Sphere(Point center, double radius){
        this->center = center;
        this->radius = radius;
    }

    virtual void draw(){
        // draw the sphere with respect to center and radius
        // std::cout << "Drawing sphere" << std::endl;
        glColor3f(color.r, color.g, color.b);
        glPushMatrix();
            glTranslatef(center.x, center.y, center.z);
            glutSolidSphere(radius, 100, 100);
        glPopMatrix();
    }

    virtual double intersect(Ray* ray){
        // Using determinant to find intersection
        // if a ray intersects the surface of the sphere then return the distance
        // from the source of the ray to the intersection point
        // otherwise return -1
        double a = ray->direction.dot(ray->direction);
        double b = 2 * ray->direction.dot(ray->start - center);
        double c = (ray->start - center).dot(ray->start - center) - radius*radius;
        double det = b * b - 4 * a * c;
        if (det < 0) return -1;
        double t1 = (-b + sqrt(det)) / (2 * a);
        double t2 = (-b - sqrt(det)) / (2 * a);
        if (t1 < 0 && t2 < 0) return -1;
        if (t1 < 0) {
            ray->intersectionPoint = ray->start + ray->direction * t2;
            return t2;
        }
        if (t2 < 0) {
            ray->intersectionPoint = ray->start + ray->direction * t1;
            return t1;
        }
        ray->intersectionPoint = ray->start + ray->direction * std::min(t1, t2);
        return std::min(t1, t2);
    }

    virtual Point normalAt(Point point){
        // find normal at point on sphere surface
        Point normal = point - center;
        normal.normalize();
        return normal;
    }
};

#endif // SPHERE_HPP