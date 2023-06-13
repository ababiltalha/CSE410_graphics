#include<iostream>
#include<GL/glut.h>
#include<math.h>
#include<ctime>

#define PI acos(-1.0)
#define degToRad(x) (x * PI / 180.0)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

using namespace std;

typedef struct {
    double x, y;
} Point;

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
    drawCircle(clockCenter, clockRadius);
    drawCircle(clockCenter, clockRadius*0.75);
    drawPoint(clockCenter);
    // show clock marks
    for(int i=0; i<60; i++){
        double theta= degToRad((double)i*6);
        double x= (clockRadius*0.9) * cos(theta);
        double y= (clockRadius*0.9) * sin(theta);
        int markerSize= 1;
        if(i%5==0) markerSize= 2;
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
    Point pendulumCenter = {0, -0.2};
    double pendulumRadius = 0.1;
    // drawLine({pendulumCenter.x, pendulumCenter.y-pendulumRadius}, {pendulumCenter.x, pendulumCenter.y-0.8});
}

//! draw pendulum


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
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);  
    glutInitWindowPosition(0, 0);
    glutCreateWindow("It's Clockin' Time");
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}