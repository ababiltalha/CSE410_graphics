#include <GL/glut.h>
#include <cmath>

// Constants
const double PI = 3.14159265358979323846;
const double GRAVITY = 9.81;

// Pendulum variables
double pendulumLength = 1.0;  // Length of the pendulum in meters
double pendulumAngle = PI / 4.0;  // Initial angle of the pendulum in radians
double pendulumAngularVelocity = 0.0;  // Initial angular velocity of the pendulum in radians per second

// Function to update the pendulum angle based on time
void updatePendulum(double deltaTime) {
    double angularAcceleration = -GRAVITY / pendulumLength * sin(pendulumAngle);
    pendulumAngularVelocity += angularAcceleration * deltaTime;
    pendulumAngle += pendulumAngularVelocity * deltaTime;
}

// Function to render the pendulum
void renderPendulum() {
    glPushMatrix();
    glRotatef(pendulumAngle * 180.0 / PI, 0.0, 0.0, 1.0);  // Rotate the pendulum based on the current angle

    // Draw the pendulum rod
    glColor3f(1.0, 1.0, 1.0);  // White color
    glBegin(GL_LINES);
    glVertex2f(0.0, 0.0);
    glVertex2f(0.0, -pendulumLength);
    glEnd();

    // Draw the pendulum bob
    glColor3f(0.8, 0.2, 0.2);  // Red color
    glTranslatef(0.0, -pendulumLength, 0.0);
    glutSolidSphere(0.1, 20, 20);

    glPopMatrix();
}

// Function to handle display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    renderPendulum();

    glutSwapBuffers();
}

// Function to handle idle updates
void idle() {
    // Calculate time elapsed since the last frame
    static double previousTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double deltaTime = currentTime - previousTime;

    // Update the pendulum
    updatePendulum(deltaTime);

    // Update the previous time
    previousTime = currentTime;

    glutPostRedisplay();
}

// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5 * (GLfloat)height / (GLfloat)width, 1.5 * (GLfloat)height / (GLfloat)width, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Second Pendulum");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
   
}