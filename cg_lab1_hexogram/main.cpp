#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void draw_with_triangles() {

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.0, 0.0);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0.0, 0.5);

    glColor3f(1.0, 0.5, 0.0);
    glVertex2f(-0.1, 0.3);

    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(-0.4, 0.3);

    glColor3f(0.5, 1.0, 0.0);
    glVertex2f(-0.25, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(-0.4, -0.3);

    glColor3f(0.0, 1.0, 0.5);
    glVertex2f(-0.1, -0.3);

    glColor3f(0.0, 1.0, 1.0);
    glVertex2f(0.0, -0.5);

    glColor3f(0.0, 0.5, 1.0);
    glVertex2f(0.1, -0.3);

    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.4, -0.3);

    glColor3f(0.5, 0.0, 1.0);
    glVertex2f(0.25, 0.0);

    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0.4, 0.3);

    glColor3f(1.0, 0.0, 0.5);
    glVertex2f(0.1, 0.3);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0.0, 0.5);

    glEnd();
}

void draw_with_quads() {
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.0, 0.0);

    glColor3f(0.0, 0.5, 1.0);
    glVertex2f(0.1, 0.3);

    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.0, 0.5);

    glColor3f(0.5, 0.0, 1.0);
    glVertex2f(-0.1, 0.3);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.5);

    glColor3f(0.0, 0.5, 1.0);
    glVertex3f(0.1, 0.3, 0.5);

    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(0.4, 0.3, 0.5);

    glColor3f(0.0, 1.0, 0.5);
    glVertex3f(0.25, 0.0, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.5);

    glColor3f(0.0, 1.0, 0.5);
    glVertex3f(0.25, 0.0, 0.5);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.4, -0.3, 0.5);

    glColor3f(0.5, 1.0, 0.0);
    glVertex3f(0.1, -0.3, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.5);

    glColor3f(0.5, 1.0, 0.0);
    glVertex3f(0.1, -0.3, 0.5);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, -0.5, 0.5);

    glColor3f(1.0, 0.5, 0.0);
    glVertex3f(-0.1, -0.3, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.5);

    glColor3f(1.0, 0.5, 0.0);
    glVertex3f(-0.1, -0.3, 0.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.4, -0.3, 0.5);

    glColor3f(1.0, 0.0, 0.5);
    glVertex3f(-0.25, 0.0, 0.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.5);

    glColor3f(1.0, 0.0, 0.5);
    glVertex3f(-0.25, 0.0, 0.5);

    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.4, 0.3, 0.5);

    glColor3f(0.5, 0.0, 1.0);
    glVertex3f(-0.1, 0.3, 0.5);

    glEnd();
}

void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    draw_with_triangles();
//    draw_with_quads();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    int windowHandle = glutCreateWindow("CG Lab 01");
    glutReshapeWindow(480, 480);
    
    glutDisplayFunc(redraw);
    
    glutMainLoop();
    
    return 0;
}
