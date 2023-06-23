#include <GL/glut.h>
#include <cmath>

const float PI = 3.14159265358979323846;

void drawCylinderSegment(float angle) {
    const int numSegments = 100;
    const float segmentAngle = angle * PI / 180.0f;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numSegments; ++i) {
        float theta = i * segmentAngle / numSegments;

        float x = cos(theta);
        float z = sin(theta);

        glVertex3f(x, 0.0f, z);
        glVertex3f(x, 1.0f, z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, -0.5f, -2.0f);
    glRotatef(70.53f, 1.0f, 0.0f, 0.0f); // Rotate the cylinder segment by 70.53 degrees around the x-axis
    glScalef(0.3f, 0.3f, 0.3f); // Scale the cylinder segment by 0.5 in all directions
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    drawCylinderSegment(70.53f);

    glFlush();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cylinder Segment");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
