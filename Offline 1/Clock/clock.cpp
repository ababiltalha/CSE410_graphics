#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include<ctime>
#include <chrono>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define radToDeg(x) (x * 180.0 / PI)
#define GRAVITY 9.8

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct {
    double x, y;
} Point;

Point pendulumBase = {0, -0.2};
double pendulumLength = 0.5;
Point pendulumCenter;
double pendulumRadius = 0.1;

double pendulumTheta = 30;
double pendulumOmega = 0;

bool nightMode = true;

using namespace std;

void drawPoint(Point p, double size=5){
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2d(p.x, p.y);
    glEnd();
}

void drawCircle(Point center, double radius, double width=5){
    glLineWidth(width);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i++){
        double theta= degToRad((double)i);
        double x= radius * cos(theta);
        double y= radius * sin(theta);
        glVertex2d(x+center.x, y+center.y);
    }
    glEnd();
}

void drawLine(Point a, Point b, double line_width=5){
    glLineWidth(line_width);
    glBegin(GL_LINES);
    glVertex2d(a.x, a.y);
    glVertex2d(b.x, b.y);
    glEnd();
}

void drawLine(Point a, double length, double theta, double width=5){
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2d(a.x, a.y);
    theta= degToRad(theta);
    glVertex2d(a.x+length*cos(theta), a.y+length*sin(theta));
    glEnd();
}

void drawClockDial(Point clockCenter){
    double clockRadius = 0.5;
    if (nightMode) glColor3f(0.0, 0.502, 0.502);
    else glColor3f(0.9, 0.647, 0.0);
    drawCircle(clockCenter, clockRadius);
    if (nightMode) glColor3f(0.0, 0.251, 0.251);
    else glColor3f(0.4, 0.243, 0.0);
    drawCircle(clockCenter, clockRadius*0.75);
    // if (nightMode) glColor3f(0.0, 0.502, 0.502);
    // else glColor3f(1.0, 0.647, 0.0);
    // drawPoint(clockCenter, 15);
    if (nightMode) glColor3f(0.0, 1.0, 1.0);
    else glColor3f(1.0, 1.0, 0.0);
    // show clock marks
    for(int i=0; i<60; i++){
        double theta= degToRad((double)i*6);
        double x= (clockRadius*0.9) * cos(theta);
        double y= (clockRadius*0.9) * sin(theta);
        int markerSize= 2;
        if(i%5==0) markerSize= 4;
        // glColor3f(0.0, 0.0, 1.0);
        drawPoint({x+clockCenter.x, y+clockCenter.y}, markerSize);
    }

}

void drawClockHands(Point clockCenter, double hDeg, double mDeg, double sDeg){
    double hourHandLength = 0.2;
    double minuteHandLength = 0.3;
    double secondHandLength = 0.4;
    double hourHandWidth = 5;
    double minuteHandWidth = 3;
    double secondHandWidth = 1;
    if (nightMode) glColor3f(0.0, 0.502, 0.502);
    else glColor3f(0.90, 0.647, 0.0);
    drawLine(clockCenter, hourHandLength, hDeg, hourHandWidth);
    drawLine(clockCenter, minuteHandLength, mDeg, minuteHandWidth);
    drawLine(clockCenter, secondHandLength, sDeg, secondHandWidth);
}

void drawClock(double hour, double minute, double second){
    Point clockCenter = {0, 0.3};

    double hDeg = 90-(((int)hour%12)+(minute/60))*30;
    double mDeg = 90-(minute+second/60)*6;
    double sDeg = 90-(second*6);

    drawClockDial(clockCenter);
    drawClockHands(clockCenter, hDeg, mDeg, sDeg);
}

void drawPendulum(){
    pendulumCenter = {pendulumBase.x + (pendulumLength+pendulumRadius) * sin(degToRad(pendulumTheta)), 
        pendulumBase.y - (pendulumLength+pendulumRadius) * cos(degToRad(pendulumTheta))};
    
    if (nightMode) glColor3f(0.0, 0.626, 0.626);
    else glColor3f(1.0, 0.864, 0.0);
    drawLine(pendulumBase, pendulumCenter);
    if (nightMode) glColor3f(0.0, 0.502, 0.502);
    else glColor3f(0.90, 0.647, 0.0);
    drawCircle(pendulumCenter, pendulumRadius);
    // cout << pendulumOmega << endl;
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    if (nightMode) glClearColor(0.0f, 0.1f, 0.1f, 0.1f);
    else glClearColor(0.5f, 0.2f, 0.2f, 0.9f);
    
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
  
    double hour = localTime->tm_hour;
    double minute = localTime->tm_min;
    double second = localTime->tm_sec;

    drawClock(hour, minute, second);
    drawPendulum();
    glFlush();
}

void idle(){
    // Calculate time elapsed since the last frame
    static double previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double deltaTime = currentTime - previousTime;

    // Update the pendulum
    // updatePendulum(deltaTime);
    double angularAcceleration = -GRAVITY / pendulumLength * sin(degToRad(pendulumTheta));
    pendulumOmega += angularAcceleration * deltaTime;
    pendulumTheta += radToDeg(pendulumOmega * deltaTime);

    // Update the previous time
    previousTime = currentTime;

    glutPostRedisplay();
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int xx, int yy)
{
    switch (key){

    case 'a': 
        nightMode = !nightMode;
        break;

    default:
        return;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);  
    glutInitWindowPosition(0, 0);
    glutCreateWindow("It's Clockin' Time");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardListener);

    glutMainLoop();
    return 0;
}