#include<iostream>
#include<GL/glut.h>

using namespace std;

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2d(10, 10);
    glEnd();
    glFlush();
}

void idle(){
    glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(700, 100);
    glutCreateWindow("Practice 1");
    glutDisplayFunc(display); // display is a function which will be called in the main loop
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}