#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include<ctime>
#include <chrono>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)
#define GRAVITY 9.8/1000000

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct {
    double x, y;
} Point;

Point pendulumBase = {0, -0.2};
double pendulumLength = 0.2;
Point pendulumCenter;
double pendulumRadius = 0.1;

double pendulumTheta = 30;
double pendulumAngularVelocity = 0;
double pendulumAngularAcceleration = -GRAVITY*sin(degToRad(pendulumTheta))/pendulumLength ;


using namespace std;

long long getCurrentTimeMilliseconds() {
    auto currentTime = std::chrono::system_clock::now();
    auto duration = currentTime.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    auto remainder = std::chrono::duration_cast<std::chrono::milliseconds>(milliseconds) % 1000;
    return remainder.count();
}



// Point clockCenter = {0, 0};

void drawPoint(Point p, double size=5){
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2d(p.x, p.y);
    glEnd();
}

void drawCircle(Point center, double radius, double width=5){
    glLineWidth(width);
    glBegin(GL_LINE_LOOP);
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
    glColor3f(0.0, 1.0, 1.0);
    drawCircle(clockCenter, clockRadius);
    glColor3f(0.0, 0.8, 0.8);
    drawCircle(clockCenter, clockRadius*0.75);
    glColor3f(0.0, 0.3, 0.3);
    drawPoint(clockCenter);
    glColor3f(1.0, 1.0, 1.0);
    // show clock marks
    for(int i=0; i<60; i++){
        double theta= degToRad((double)i*6);
        double x= (clockRadius*0.9) * cos(theta);
        double y= (clockRadius*0.9) * sin(theta);
        int markerSize= 1;
        if(i%5==0) markerSize= 2;
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

//! draw pendulum
void drawPendulum(){
    pendulumAngularAcceleration = -GRAVITY*sin(degToRad(pendulumTheta))/pendulumLength ;
    pendulumAngularVelocity += pendulumAngularAcceleration;
    pendulumTheta += pendulumAngularVelocity;

    pendulumCenter = {pendulumBase.x+(pendulumLength+pendulumRadius)*sin(degToRad(pendulumTheta)), 
        pendulumBase.y-(pendulumLength+pendulumRadius)*cos(degToRad(pendulumTheta))};
    
    // drawLine(pendulumBase, pendulumCenter);
    drawLine(pendulumBase, pendulumLength, 270+pendulumTheta);
    drawCircle(pendulumCenter, pendulumRadius);
    cout << pendulumAngularAcceleration << endl;
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    
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
    glutPostRedisplay();
}


int main(int argc, char** argv){
    // start_time= getCurrentTimeMilliseconds();

    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);  
    glutInitWindowPosition(0, 0);
    glutCreateWindow("It's Clockin' Time");
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}