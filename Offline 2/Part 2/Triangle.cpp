#ifndef TRIANGLE_CPP
#define TRIANGLE_CPP

#include "Point.cpp"
#include "Transformation.cpp"

using namespace std;

// random number generator 
// to assign color components of a triangle
static unsigned long int g_seed = 1; 
inline int randomInt() {     
    g_seed = (214013 * g_seed + 2531011);     
    return (g_seed >> 16) & 0x7FFF; 
} 

class Triangle {
public:
    Point v1, v2, v3;
    int color[3]; // 0 = red, 1 = green, 2 = blue

    Triangle(Point v1, Point v2, Point v3) : v1(v1), v2(v2), v3(v3) {
        color[0] = randomInt() % 256;
        color[1] = randomInt() % 256;
        color[2] = randomInt() % 256;
    }

    Point getNormalToPlane() {
        Point a = v2 - v1;
        Point b = v3 - v1;
        Point n = a.cross(b);
        double norm = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        return Point(n.x / norm, n.y / norm, n.z / norm);
    }

    Point getCentroid(){
        return Point((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3, (v1.z + v2.z + v3.z) / 3);
    }

    void print() {
        // cout << fixed << setprecision(7);
        cout << "R " << color[0] << " G " << color[1] << " B " << color[2] << endl;
        cout << v1.x << " " << v1.y << " " << v1.z << endl;
        cout << v2.x << " " << v2.y << " " << v2.z << endl;
        cout << v3.x << " " << v3.y << " " << v3.z << endl;
        cout << endl;
    }
};





#endif