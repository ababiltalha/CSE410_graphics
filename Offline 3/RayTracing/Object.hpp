#ifndef OBJECT_HPP
#define OBJECT_HPP

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)
#define EPSILON 0.0005

#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3

#include <vector>
#include "Point.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "LightSource.hpp"
#include "NormalLight.hpp"
#include "SpotLight.hpp"

using namespace std;

class Object {
public:
    Color color; // RGB
    double coEfficients[4]; // ambient, diffuse, specular, reflection
    int shininess; // exponent term of specular equation

    void setColor(Color color){
        this->color = color;
    }
    void setCoEfficients(double coEfficients[]){
        for (int i = 0; i < 4; ++i) {
            this->coEfficients[i] = coEfficients[i];
        }
    }
    void setShininess(int shininess){
        this->shininess = shininess;
    }

    virtual void draw() = 0;

    virtual double intersect(Ray* ray) = 0;

    virtual Color getColor(Point point){
        return color;
    }

    virtual Color getAmbientColor(Point intersectionPoint){
        // return Color((normalAt(intersectionPoint).x+1)/2, (normalAt(intersectionPoint).y+1)/2, (normalAt(intersectionPoint).z+1)/2);
        return getColor(intersectionPoint) * coEfficients[AMBIENT];
    }

    virtual Color getDiffuseAndSpecularColor(Point intersectionPoint, vector<LightSource*> lights, vector<Object*> objects, Ray* ray){
        double lambert = 0;
        double phong = 0;
        for (const auto& light : lights) {
            Ray toSource(intersectionPoint, light->position - intersectionPoint);
            Point advancedStart = intersectionPoint + toSource.direction * EPSILON;
            toSource.setStart(advancedStart);
            assert(toSource.direction.length() < 1.1 && toSource.direction.length() > 0.9);

            bool isShadow = false;
            if (light->type == 1) {
                // spot light
                SpotLight* spotLight = (SpotLight*) light;
                Ray toPoint(spotLight->position, intersectionPoint - spotLight->position);
                double angle = acos(toPoint.direction.dot(spotLight->direction));
                if (angle > spotLight->cutoffAngle) {
                    continue;
                }
            }

            for (const auto &object : objects) {
                double t = object->intersect(&toSource);
                if (t > 0) {
                    isShadow = true;
                    break;
                }
            }

            if (!isShadow) {
                Point normal = normalAt(intersectionPoint);
                double distance = intersectionPoint.distance(light->position);
                double scalingFactor = exp(-light->falloff * distance * distance);
                lambert += scalingFactor * max(0.0, normal.dot(toSource.direction));
                // reflected ray from incident ray from camera on this point
                Point reflectedRay = ray->direction - normal * 2 * ray->direction.dot(normal);
                reflectedRay.normalize();
                // return Color(reflectedRay.x, reflectedRay.y, reflectedRay.z);
                phong += scalingFactor * pow(max(0.0, reflectedRay.dot(toSource.direction)), shininess);
            }
        }
        Color diffuse = getColor(intersectionPoint) * coEfficients[DIFFUSE] * lambert;
        Color specular = getColor(intersectionPoint) * coEfficients[SPECULAR] * phong;
        return diffuse + specular; // new color 
    }

    virtual Point normalAt(Point point) = 0;

    virtual Color getReflectedColor(Point intersectionPoint, vector<LightSource*> lights, vector<Object*> objects, Ray* ray, int recursionLevel){
        Color reflectedColor(0, 0, 0);
        Point currentIntersectionPoint = intersectionPoint;
        while(recursionLevel > 0){
            recursionLevel--;
            Point normal = normalAt(currentIntersectionPoint);
            Point reflectedRay = ray->direction - normal * 2 * ray->direction.dot(normal);
            reflectedRay.normalize();
            Ray reflected(currentIntersectionPoint, reflectedRay);
            Point advancedStart = currentIntersectionPoint + reflected.direction * EPSILON;
            reflected.setStart(advancedStart);
            // assert(reflected.direction.length() < 1.1 && reflected.direction.length() > 0.9);
            double minT = 1000000000;
            Object* closestObject = NULL;
            for (const auto &object : objects) {
                double t = object->intersect(&reflected);
                if (t > 0 && t < minT) {
                    minT = t;
                    closestObject = object;
                }
            }
            if (closestObject == NULL) {
                reflectedColor = reflectedColor + Color(0, 0, 0); 
                break;
            }
            currentIntersectionPoint = reflected.start + reflected.direction * minT;
            reflectedColor = reflectedColor + (closestObject->getAmbientColor(currentIntersectionPoint)
                            + closestObject->getDiffuseAndSpecularColor(currentIntersectionPoint, lights, objects, &reflected)) 
                            * closestObject->coEfficients[REFLECTION];

        }

        return reflectedColor * coEfficients[REFLECTION];
    }
    

};

#endif // OBJECT_HPP