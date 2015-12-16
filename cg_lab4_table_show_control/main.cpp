#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include "stanford_bunny.h"

float fRotate;
float fDistance = 0.2f;

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

bool bDrawList = false;
int objectVisibility = 4;

int wHeight = 0;
int wWidth = 0;

GLuint tableList = 0;
GLuint teapotList = 0;
GLuint bunnyList = 0;

void drawLeg();

GLuint genTableList() {
    GLuint table = glGenLists(1);
    glNewList(table, GL_COMPILE);

    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, 1, 1.5);
    drawLeg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    drawLeg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    drawLeg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    drawLeg();
    glPopMatrix();

    glEndList();

    return table;
}

GLuint genTeapotList() {
    GLuint teapot = glGenLists(2);
    glNewList(teapot, GL_COMPILE);

    glPushMatrix();
    glTranslatef(-1.5, 0, 4 + 1);
    glRotatef(90, 1, 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();

    glEndList();

    return teapot;
}

GLuint genBunnyList() {
    GLuint bunny = glGenLists(3);
    glNewList(bunny, GL_COMPILE);

    glPushMatrix();
    glTranslatef(1.5, 0, 4 + 1);
    glRotatef(90, 1, 0, 0);
    glScalef(4, 4, 4);
    glColor3f(0.5, 0.5, 0.5);
    DrawBunny();
    glPopMatrix();

    glEndList();

    return bunny;
}

void drawObjectList() {

    if (objectVisibility & 1) {
        glCallList(tableList);
    }

    if (objectVisibility & 2) {
        glCallList(teapotList);
    }

    if (objectVisibility & 4) {
        glCallList(bunnyList);
    }
}

void drawObjects() // This function draws a table and bunny with RGB colors
{

    if (objectVisibility & 4) {
        glPushMatrix();
        glTranslatef(1.5, 0, 4 + 1);
        glRotatef(90, 1, 0, 0);
        glScalef(4, 4, 4);
        glColor3f(0.5, 0.5, 0.5);
        DrawBunny();
        glPopMatrix();
    }

    if (objectVisibility & 2) {
        glPushMatrix();
        glTranslatef(-1.5, 0, 4 + 1);
        glRotatef(90, 1, 0, 0);
        glutSolidTeapot(1);
        glPopMatrix();
    }

    if (objectVisibility & 1) {
        glPushMatrix();
        glTranslatef(0, 0, 3.5);
        glScalef(5, 4, 1);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(1.5, 1, 1.5);
        drawLeg();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.5, 1, 1.5);
        drawLeg();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(1.5, -1, 1.5);
        drawLeg();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1.5, -1, 1.5);
        drawLeg();
        glPopMatrix();
    }
}

void drawLeg() {
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}

void updateView(int width, int height) {
    glViewport(0, 0, width, height);                        // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

    float whRatio = (GLfloat) width / (GLfloat) height;
    if (bPersp == 1)
        gluPerspective(45, 1, 1, 100);
    else
        glOrtho(-3, 3, -3, 3, -100, 100);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
}

void reshape(int width, int height) {
    if (height == 0)                                        // Prevent A Divide By Zero By
    {
        height = 1;                                        // Making Height Equal One
    }

    wHeight = height;
    wWidth = width;

    updateView(wWidth, wHeight);
}

void idle() {
    glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y) {
    switch (k) {
        case 27:
        case 'q': {
            exit(0);
            break;
        }
        case 'p': {
            bPersp = !bPersp;
            updateView(wWidth, wHeight);
            break;
        }

        case ' ': {
            bAnim = !bAnim;
            break;
        }
        case 'o': {
            bWire = !bWire;
            break;
        }

        case 'a': {
            eye[0] = eye[0] + fDistance;
            center[0] = center[0] + fDistance;
            break;
        }
        case 'd': {
            eye[0] = eye[0] - fDistance;
            center[0] = center[0] - fDistance;
            break;
        }
        case 'w': {
            eye[1] = eye[1] - fDistance;
            center[1] = center[1] - fDistance;
            break;
        }
        case 's': {
            eye[1] = eye[1] + fDistance;
            center[1] = center[1] + fDistance;
            break;
        }
        case 'z': {
            eye[2] = (float) (eye[2] * 0.95);
            break;
        }
        case 'c': {
            eye[2] = (float) (eye[2] * 1.05);
            break;
        }
        case 'l':
            bDrawList = !bDrawList;
            break;
        case 'v':
            objectVisibility = (objectVisibility + 1) % 8;
            break;
        default:break;
    }
}

void getFPS() {
    static int frame = 0, time, timebase = 0;
    static char buffer[256];

    char mode[64];
    if (bDrawList)
        strcpy(mode, "display list");
    else
        strcpy(mode, "naive");

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        sprintf(buffer, "FPS:%4.2f %s",
                frame * 1000.0 / (time - timebase), mode);
        timebase = time;
        frame = 0;
    }

    //glutSetWindowTitle(buffer);
    char *c;
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 480, 0, 480, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(10, 10);
    for (c = buffer; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void redraw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);

    if (bWire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_pos[] = {5, 5, 5, 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glEnable(GL_LIGHT0);

    glRotatef(fRotate, 0, 1.0f, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);

    if (!bDrawList)
        drawObjects();
    else
        drawObjectList();

    if (bAnim) fRotate += 0.5f;

    getFPS();

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(480, 480);
    int windowHandle = glutCreateWindow("CG Lab4 - Display Lists");

    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    CGLContextObj ctx = CGLGetCurrentContext();
    const int interval = 0;

    CGLSetParameter(ctx, kCGLCPSwapInterval, &interval);

    tableList = genTableList();
    teapotList = genTeapotList();
    bunnyList = genBunnyList();

    glutMainLoop();
    return 0;
}


