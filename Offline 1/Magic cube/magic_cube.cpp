#include <iostream>
#include <GL/glut.h>
#include <math.h>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640


typedef struct
{
    GLdouble x, y, z;
} Point;

// Global variables
Point eyePos;   // pos
Point lookDir;  // l
Point rightDir; // r
Point upDir;    // u
Point center;

const double Vertex_dec = 1.0/24;
const double Vertex_inc = 1.0/48;
const double OneBy3 = 0.333333;

bool isAxes = true;
bool isFillTriangle = true;

// Vertices of front facing triangle
Point vx;
Point vy;
Point vz;

using namespace std;

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

void colorBlue()
{
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
}

void colorRed()
{
    glColor3f(1.0f, 0.0f, 0.0f); // Red
}

void colorGreen()
{
    glColor3f(0.0f, 1.0f, 0.0f); // Green
}

void point(Point A)
{
    glVertex3f(A.x, A.y, A.z);
}

void drawTriangle()
{
    

    if (isFillTriangle) glBegin(GL_TRIANGLES);
    else glBegin(GL_LINE_LOOP);
    // Front
    colorBlue();
    point(vx);
    colorRed();
    point(vy);
    // colorGreen();
    point(vz);
    glEnd();
}

// void drawLineTriangle()
// {
//     glBegin(GL_LINE_LOOP);
//     // Front
//     colorBlue();
//     point(vx);
//     colorRed();
//     point(vy);
//     colorGreen();
//     point(vz);
//     glEnd();
// }

void drawPyramid()
{
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(i * 90, 0, 1, 0);
        drawTriangle();
        glPopMatrix();
    }
}

void drawOctahedron()
{
    drawPyramid();
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    drawPyramid();
    glPopMatrix();
}

void drawAxes()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();
}

//! display()
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    center = {eyePos.x + lookDir.x, eyePos.y + lookDir.y, eyePos.z + lookDir.z};
    rightDir = {upDir.z * (lookDir.y - eyePos.y) - upDir.y * (lookDir.z - eyePos.z), 
                upDir.x * (lookDir.z - eyePos.z) - upDir.z * (lookDir.x - eyePos.x), 
                upDir.y * (lookDir.x - eyePos.x) - upDir.x * (lookDir.y - eyePos.y)};
    // control viewing (or camera)
    gluLookAt(eyePos.x, eyePos.y, eyePos.z,
              center.x, center.y, center.z,
              upDir.x, upDir.y, upDir.z);
    // draw
    if (isAxes)
        drawAxes();
    drawOctahedron();
    glutSwapBuffers(); // Render now
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int xx, int yy)
{
    double rate = degToRad(1);
    double s, v = 0.01;
    switch (key){

    case '1': // look left
        rightDir.x = rightDir.x * cos(rate) + lookDir.x * sin(rate);
        rightDir.y = rightDir.y * cos(rate) + lookDir.y * sin(rate);
        rightDir.z = rightDir.z * cos(rate) + lookDir.z * sin(rate);

        lookDir.x = lookDir.x * cos(rate) - rightDir.x * sin(rate);
        lookDir.y = lookDir.y * cos(rate) - rightDir.y * sin(rate);
        lookDir.z = lookDir.z * cos(rate) - rightDir.z * sin(rate);
        break;

    case '2': // look right
        rightDir.x = rightDir.x * cos(-rate) + lookDir.x * sin(-rate);
        rightDir.y = rightDir.y * cos(-rate) + lookDir.y * sin(-rate);
        rightDir.z = rightDir.z * cos(-rate) + lookDir.z * sin(-rate);

        lookDir.x = lookDir.x * cos(-rate) - rightDir.x * sin(-rate);
        lookDir.y = lookDir.y * cos(-rate) - rightDir.y * sin(-rate);
        lookDir.z = lookDir.z * cos(-rate) - rightDir.z * sin(-rate);
        break;

    case '3': // look up
        lookDir.x = lookDir.x * cos(rate) + upDir.x * sin(rate);
        lookDir.y = lookDir.y * cos(rate) + upDir.y * sin(rate);
        lookDir.z = lookDir.z * cos(rate) + upDir.z * sin(rate);

        upDir.x = upDir.x * cos(rate) - lookDir.x * sin(rate);
        upDir.y = upDir.y * cos(rate) - lookDir.y * sin(rate);
        upDir.z = upDir.z * cos(rate) - lookDir.z * sin(rate);
        break;

    case '4': // look down
        lookDir.x = lookDir.x * cos(-rate) + upDir.x * sin(-rate);
        lookDir.y = lookDir.y * cos(-rate) + upDir.y * sin(-rate);
        lookDir.z = lookDir.z * cos(-rate) + upDir.z * sin(-rate);

        upDir.x = upDir.x * cos(-rate) - lookDir.x * sin(-rate);
        upDir.y = upDir.y * cos(-rate) - lookDir.y * sin(-rate);
        upDir.z = upDir.z * cos(-rate) - lookDir.z * sin(-rate);
        break;

    case '5': // tilt counter-clockwise
        upDir.x = upDir.x * cos(rate) + rightDir.x * sin(rate);
        upDir.y = upDir.y * cos(rate) + rightDir.y * sin(rate);
        upDir.z = upDir.z * cos(rate) + rightDir.z * sin(rate);

        rightDir.x = rightDir.x * cos(rate) - upDir.x * sin(rate);
        rightDir.y = rightDir.y * cos(rate) - upDir.y * sin(rate);
        rightDir.z = rightDir.z * cos(rate) - upDir.z * sin(rate);
        break;

    case '6': // tilt clockwise
        upDir.x = upDir.x * cos(-rate) + rightDir.x * sin(-rate);
        upDir.y = upDir.y * cos(-rate) + rightDir.y * sin(-rate);
        upDir.z = upDir.z * cos(-rate) + rightDir.z * sin(-rate);

        rightDir.x = rightDir.x * cos(-rate) - upDir.x * sin(-rate);
        rightDir.y = rightDir.y * cos(-rate) - upDir.y * sin(-rate);
        rightDir.z = rightDir.z * cos(-rate) - upDir.z * sin(-rate);
        break;

    case 'a':
        eyePos.x += (upDir.y * lookDir.z) * v;
        eyePos.z += (-lookDir.x *upDir.y) * v;
        // s = sqrt(eyePos.x * eyePos.x + eyePos.z * eyePos.z) / (4 * sqrt(2));
        // eyePos.x /= s;
        // eyePos.z /= s;
        
        break;

    case 'd':
        eyePos.x += (-upDir.y * lookDir.z) * v;
        eyePos.z += (lookDir.x * upDir.y) * v;
        // s = sqrt(eyePos.x * eyePos.x + eyePos.z * eyePos.z) / (4 * sqrt(2));
        // eyePos.x /= s;
        // eyePos.z /= s;
        break;

    case 'r':
        eyePos = {2, 2, 2};     // pos
        lookDir = {-2, -2, -2}; // l
        upDir = {0, 1, 0};      // u
        rightDir = {upDir.z * (lookDir.y - eyePos.y) - upDir.y * (lookDir.z - eyePos.z), 
                    upDir.x * (lookDir.z - eyePos.z) - upDir.z * (lookDir.x - eyePos.x), 
                    upDir.y * (lookDir.x - eyePos.x) - upDir.x * (lookDir.y - eyePos.y)}; // r, cross product of l and u
        break;

    case 'f':
        isFillTriangle = !isFillTriangle;
        break;
    
    case 'x':
        isAxes = !isAxes;
        break;

    case ',': // shrink to 1/3
        if (vx.x <= OneBy3 || vy.y <= OneBy3 || vz.z <= OneBy3)
            break;
        if (vx.y >= OneBy3 || vx.z >= OneBy3 || vy.x >= OneBy3 || vy.z >= OneBy3 || vz.x >= OneBy3 || vz.y >= OneBy3)
            break;
        vx.x -= Vertex_dec; vx.y += Vertex_inc; vx.z += Vertex_inc;
        vy.x += Vertex_inc; vy.y -= Vertex_dec; vy.z += Vertex_inc;
        vz.x += Vertex_inc; vz.y += Vertex_inc; vz.z -= Vertex_dec;
        break;
    
    case '.': // enlarge from 1/3
        if (vx.x >= 1 || vy.y >= 1 || vz.z >= 1)
            break;
        if (vx.y <= 0 || vx.z <= 0 || vy.x <= 0 || vy.z <= 0 || vz.x <= 0 || vz.y <= 0)
            break;
        vx.x += Vertex_dec; vx.y -= Vertex_inc; vx.z -= Vertex_inc;
        vy.x -= Vertex_inc; vy.y += Vertex_dec; vy.z -= Vertex_inc;
        vz.x -= Vertex_inc; vz.y -= Vertex_inc; vz.z += Vertex_dec;
        break; 

    default:
        return;
    }
    glutPostRedisplay();
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y)
{
    double rate = 0.01;
    switch (key){

    case GLUT_KEY_UP: 
        eyePos.x += lookDir.x * rate;
        eyePos.y += lookDir.y * rate;
        eyePos.z += lookDir.z * rate;
        break;
    case GLUT_KEY_DOWN: 
        eyePos.x -= lookDir.x * rate;
        eyePos.y -= lookDir.y * rate;
        eyePos.z -= lookDir.z * rate;
        break;

    case GLUT_KEY_RIGHT:
        eyePos.x += rightDir.x * rate;
        eyePos.y += rightDir.y * rate;
        eyePos.z += rightDir.z * rate;
        break;
    case GLUT_KEY_LEFT:
        eyePos.x -= rightDir.x * rate;
        eyePos.y -= rightDir.y * rate;
        eyePos.z -= rightDir.z * rate;
        break;

    case GLUT_KEY_PAGE_UP:
        eyePos.x += upDir.x * rate;
        eyePos.y += upDir.y * rate;
        eyePos.z += upDir.z * rate;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eyePos.x -= upDir.x * rate;
        eyePos.y -= upDir.y * rate;
        eyePos.z -= upDir.z * rate;
        break;

    default:
        return;
    }
    glutPostRedisplay();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char **argv)
{
    // Set up the camera
    eyePos = {2, 2, 2};     // pos
    lookDir = {-2, -2, -2}; // l
    upDir = {0, 1, 0};      // u
    rightDir = {upDir.z * (lookDir.y - eyePos.y) - upDir.y * (lookDir.z - eyePos.z), 
                upDir.x * (lookDir.z - eyePos.z) - upDir.z * (lookDir.x - eyePos.x), 
                upDir.y * (lookDir.x - eyePos.x) - upDir.x * (lookDir.y - eyePos.y)}; // r, cross product of l and u

    // Initial vertices of triangle
    vx = {1, 0, 0};
    vy = {0, 1, 0};
    vz = {0, 0, 1};

    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(1000, 100);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("It's Cubin' Time");
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    // glutIdleFunc(idle);                         // Register idle callback handler for window re-paint
    glutReshapeFunc(reshapeListener);    // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);  // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener); // Register callback handler for special-key event
    initGL();
    glutMainLoop();
    return 0;
}