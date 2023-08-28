#include <bits/stdc++.h>
#include <GL/glut.h>
#include "Point.hpp"
#include "Object.hpp"
#include "Checkerboard.hpp"
#include "Sphere.hpp"
#include "Pyramid.hpp"
#include "Cube.hpp"
#include "LightSource.hpp"
#include "NormalLight.hpp"
#include "SpotLight.hpp"

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)

#define R 0
#define G 1
#define B 2

#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3

using namespace std;

Point pos, u, r, l;
double near, far, fovY, aspectRatio;
int recursionLevel, numberOfPixels;
Checkerboard checkerboard;
int numberOfObjects;
vector<Object> objects;
int numberOfNormalLights;
vector<NormalLight> normalLights;
int numberOfSpotLights;
vector<SpotLight> spotLights;

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
    pos.x=0;    pos.y=15;    pos.z=15;
    l.x=0;      l.y=0;      l.z=-1;
    r.x=1;      r.y=0;      r.z=0;
    u = r.cross(l);
}



/*  Handler for window-repaint event. Call back when the window first appears and
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
    gluLookAt(pos.x, pos.y, pos.z,
              pos.x+l.x, pos.y+l.y, pos.z+l.z,
              u.x, u.y, u.z);
    // draw
    drawAxes();
    // draw the checkerboard
    checkerboard.draw();

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
    gluPerspective(fovY, aspect, near, far);
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



int main(int argc, char** argv)
{
    ifstream fin("description.txt");
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
        if (type == "sphere") {
            Sphere sphere;
            fin >> sphere.center.x >> sphere.center.y >> sphere.center.z;
            fin >> sphere.radius;
            fin >> sphere.color[R] >> sphere.color[G] >> sphere.color[B];
            fin >> sphere.coEfficients[AMBIENT] >> sphere.coEfficients[DIFFUSE] >> sphere.coEfficients[SPECULAR] >> sphere.coEfficients[REFLECTION];
            fin >> sphere.shininess;
            objects.push_back(sphere);
        } else if (type == "pyramid") {
            Pyramid pyramid;
            fin >> pyramid.lowest.x >> pyramid.lowest.y >> pyramid.lowest.z;
            fin >> pyramid.width >> pyramid.height;
            fin >> pyramid.color[R] >> pyramid.color[G] >> pyramid.color[B];
            fin >> pyramid.coEfficients[AMBIENT] >> pyramid.coEfficients[DIFFUSE] >> pyramid.coEfficients[SPECULAR] >> pyramid.coEfficients[REFLECTION];
            fin >> pyramid.shininess;
            objects.push_back(pyramid);
        } else if (type == "cube") {
            Cube cube;
            fin >> cube.bottomLowerLeft.x >> cube.bottomLowerLeft.y >> cube.bottomLowerLeft.z;
            fin >> cube.edge;
            fin >> cube.color[R] >> cube.color[G] >> cube.color[B];
            fin >> cube.coEfficients[AMBIENT] >> cube.coEfficients[DIFFUSE] >> cube.coEfficients[SPECULAR] >> cube.coEfficients[REFLECTION];
            fin >> cube.shininess;
            objects.push_back(cube);
        } else {
            cout << "Invalid object type" << endl;
            return 0;
        }
    }

    fin >> numberOfNormalLights;
    for (int i = 0; i < numberOfNormalLights; i++)
    {
        NormalLight normalLight;
        fin >> normalLight.position.x >> normalLight.position.y >> normalLight.position.z;
        fin >> normalLight.falloff;
        normalLights.push_back(normalLight);
    }

    fin >> numberOfSpotLights;
    for (int i = 0; i < numberOfSpotLights; i++)
    {
        SpotLight spotLight;
        fin >> spotLight.position.x >> spotLight.position.y >> spotLight.position.z;
        fin >> spotLight.falloff;
        fin >> spotLight.direction.x >> spotLight.direction.y >> spotLight.direction.z;
        fin >> spotLight.cutoffAngle;
        spotLights.push_back(spotLight);
    }

    fin.close();

    cout << "input taken" << endl;

    //



    setCamera();

    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing 2");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;

}