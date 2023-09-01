#include <bits/stdc++.h>
#include <GL/glut.h>
#include "bitmap_image.hpp"
#include "Point.hpp"
#include "Object.hpp"
#include "Checkerboard.hpp"
#include "Sphere.hpp"
#include "Pyramid.hpp"
#include "Cube.hpp"
#include "LightSource.hpp"
#include "NormalLight.hpp"
#include "SpotLight.hpp"
#include "Ray.hpp"
#include "Color.hpp"

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)

#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3

using namespace std;

string fileName = "description.txt";

Point pos, u, r, l;
double near, far, fovY, aspectRatio, fovX;
int recursionLevel, numberOfPixels;
Checkerboard checkerboard;
int numberOfObjects;
vector<Object*> objects;
vector<LightSource*> lights;
int numberOfNormalLights;
vector<NormalLight*> normalLights;
int numberOfSpotLights;
vector<SpotLight*> spotLights;
double nearHeight, nearWidth;

int windowWidth = 800;
int windowHeight = 800;

ofstream fout;

// take the information from description.txt using ifstream fin
// and store them in the global variables
void inputDescription(string fileName) {
    ifstream fin(fileName);
    fin >> near >> far >> fovY >> aspectRatio;

    fin >> recursionLevel;
    fin >> numberOfPixels;

    fin >> checkerboard.cellWidth;
    fin >> checkerboard.coEfficients[AMBIENT] >> checkerboard.coEfficients[DIFFUSE] >> checkerboard.coEfficients[REFLECTION];

    fin >> numberOfObjects;

    for (int i = 0; i < numberOfObjects; i++)
    {
        string type;
        fin >> type;
        Object* object;
        Color color;
        double coEfficients[4];
        int shininess;
        if (type == "sphere") {
            Point center;
            double radius;
            fin >> center.x >> center.y >> center.z;
            fin >> radius;
            object = new Sphere(center, radius);
            fin >> color.r >> color.g >> color.b;
            fin >> coEfficients[AMBIENT] >> coEfficients[DIFFUSE] >> coEfficients[SPECULAR] >> coEfficients[REFLECTION];
            fin >> shininess;
            object->setColor(color);
            object->setCoEfficients(coEfficients);
            object->setShininess(shininess);
            objects.push_back(object);
        } else if (type == "pyramid") {
            Point lowest;
            double width, height;
            fin >> lowest.x >> lowest.y >> lowest.z;
            fin >> width >> height;
            object = new Pyramid(lowest, width, height);
            fin >> color.r >> color.g >> color.b;
            fin >> coEfficients[AMBIENT] >> coEfficients[DIFFUSE] >> coEfficients[SPECULAR] >> coEfficients[REFLECTION];
            fin >> shininess;
            object->setColor(color);
            object->setCoEfficients(coEfficients);
            object->setShininess(shininess);
            objects.push_back(object);
        } else if (type == "cube") {
            Point bottomLowerLeft;
            double edge;
            fin >> bottomLowerLeft.x >> bottomLowerLeft.y >> bottomLowerLeft.z;
            fin >> edge;
            object = new Cube(bottomLowerLeft, edge);
            fin >> color.r >> color.g >> color.b;
            fin >> coEfficients[AMBIENT] >> coEfficients[DIFFUSE] >> coEfficients[SPECULAR] >> coEfficients[REFLECTION];
            fin >> shininess;
            object->setColor(color);
            object->setCoEfficients(coEfficients);
            object->setShininess(shininess);
            objects.push_back(object);
        } else {
            cout << "Invalid object type" << endl;
            return;
        }
    }

    objects.push_back(&checkerboard);

    fin >> numberOfNormalLights;
    for (int i = 0; i < numberOfNormalLights; i++)
    {
        NormalLight* normalLight = new NormalLight();
        fin >> normalLight->position.x >> normalLight->position.y >> normalLight->position.z;
        fin >> normalLight->falloff;
        normalLights.push_back(normalLight);
        lights.push_back(normalLight);
    }

    fin >> numberOfSpotLights;
    for (int i = 0; i < numberOfSpotLights; i++)
    {
        SpotLight* spotLight = new SpotLight();
        Point lookAt;
        fin >> spotLight->position.x >> spotLight->position.y >> spotLight->position.z;
        fin >> spotLight->falloff;
        fin >> lookAt.x >> lookAt.y >> lookAt.z;
        fin >> spotLight->cutoffAngle;
        spotLight->setDirection(lookAt - spotLight->position);
        spotLight->direction.normalize();
        spotLights.push_back(spotLight);
        lights.push_back(spotLight);
    }

    fin.close();

    fovX = aspectRatio * fovY;
    // cout << tan(PI / 4.0) << endl;
    nearHeight = 2 * near * tan(degToRad(fovY/2));
    nearWidth = 2 * near * tan(degToRad(fovX/2));
    
    cout << "Input Taken" << endl;
}

void debugInput(){
    cout << "Recursion level " << recursionLevel << endl;
    cout << "Number of pixels " << numberOfPixels << endl;
    cout << "Number of objects " << numberOfObjects << endl;
    cout << "Number of normal lights " << numberOfNormalLights << endl;
    cout << "Number of spot lights " << numberOfSpotLights << endl;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(10,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,10,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,10);
    glEnd();
}

void setCamera(){
    pos.x=0;    pos.y=150;    pos.z=50;
    l.x=0;      l.y=-1;      l.z=0;
    l.normalize();
    // cout << l.x << " " << l.y << " " << l.z << endl;
    r.x=-1;      r.y=0;      r.z=0;
    u = r.cross(l);
}

//! CAPTURE FUNCTION
// capture the current window and save it in a file
void capture(){
    // init
    bitmap_image image(numberOfPixels, numberOfPixels);
    image.set_all_channels(0, 0, 0);
    // image.save_image("test.png");

    double dw = nearWidth / numberOfPixels;
    double dh = nearHeight / numberOfPixels;
    Point middlePoint = pos + l * near;
    Point topLeft = middlePoint - r * nearWidth / 2 + u * nearHeight / 2;
    Point bottomRight = middlePoint + r * nearWidth / 2 - u * nearHeight / 2;
    u.normalize();
    r.normalize();
    Point pixel;
    cout << "Rendering starting..." << endl;
    for (int i=0; i < numberOfPixels; i++) {
        for (int j=0; j < numberOfPixels; j++) {
            pixel = topLeft + r * dw * i - u * dh * j;
            Ray ray(pixel, pixel - pos);
            double tMin = INT_MAX;
            for (const auto object : objects) {
                double t = object->intersect(&ray);
                if (t > 0 && t < tMin && t < far) {
                    tMin = t;
                    Point intersectionPoint = ray.start + ray.direction * t;
                    Color color = object->getAmbientColor(intersectionPoint);
                    color = color + object->getDiffuseAndSpecularColor(intersectionPoint, lights, objects, &ray);
                    color = color + object->getReflectedColor(intersectionPoint, lights, objects, &ray, recursionLevel);
                    color.check();
                    image.set_pixel(i, j, color.r * 255, color.g * 255, color.b * 255);
                }
            }
            
        }
        if (i % 100 == 0) {
            cout << i * 100 / numberOfPixels << "% rendered" << endl;
        }
    }
    cout << "Image rendered" << endl;
    image.save_image("test.bmp");



}

int temp = 0;

/*  //! DISPLAY FUNCTION
    Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    u = r.cross(l);
    // control viewing (or camera)
    gluLookAt(
        pos.x, pos.y, pos.z,
        pos.x+l.x, pos.y+l.y, pos.z+l.z,
        u.x, u.y, u.z
    );

    // start draw
    drawAxes();
    // draw the checkerboard
    checkerboard.drawBoard(windowWidth, windowHeight);
    // draw all the objects from the objects vector

    for (const auto object : objects) {
        object->draw();
    }

    // draw all the lights from the lights vector
    for (const auto& light : lights) {
        light->draw();
    }

    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(fovY, aspectRatio, near, far);
}

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.01;
	switch(key){

		case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;

        case '0':
            capture();
            break;

        case ' ':
            checkerboard.toggleTexture();
            if (checkerboard.isTextureOn()) cout << "Texture on" << endl;
            else cout << "Texture off" << endl;
            break;

		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
    double rate = 2.5;
	switch(key){
		case GLUT_KEY_UP:
			pos.x+=rate * l.x;
			pos.y+=rate * l.y;
			pos.z+=rate * l.z;
			break;
		case GLUT_KEY_DOWN:
			pos.x-=rate * l.x;
			pos.y-=rate * l.y;
			pos.z-=rate * l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=rate * r.x;
			pos.y+=rate * r.y;
			pos.z+=rate * r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=rate * r.x;
			pos.y-=rate * r.y;
			pos.z-=rate * r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=rate * u.x;
			pos.y+=rate * u.y;
			pos.z+=rate * u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=rate * u.x;
			pos.y-=rate * u.y;
			pos.z-=rate * u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

void clearMemory(){
    for (const auto object : objects)
    {
        delete object;
    }
    for (const auto light : lights)
    {
        delete light;
    }
}


int main(int argc, char** argv)
{
    inputDescription(fileName);
    debugInput();
    setCamera();

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(windowWidth, windowHeight);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("Tray Racing");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    clearMemory();
    return 0;

}