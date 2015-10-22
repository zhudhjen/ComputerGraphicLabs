#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f
const float pi = 3.14159265357989323846;

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

int teapot_rotation = 0;
float teapot_pos[2] = {0, 0};

void Draw_Leg()
{
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}

void Draw_Scene()
{
    glPushMatrix();
    glTranslatef(0, 0, 4+1);
    glRotatef(90, 1, 0, 0);
    glTranslatef(teapot_pos[0], 0, teapot_pos[1]);
    glRotated(teapot_rotation, 0, 1, 0);
    glutSolidTeapot(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, 1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    Draw_Leg();
    glPopMatrix();

}

void updateView(int width, int height)
{
    glViewport(0,0,width,height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    float whRatio = (GLfloat)width/(GLfloat)height;

    if (bPersp){
        gluPerspective(45.0f, 1.0f, 1.0f, 20.0f);
    }
    else
        glOrtho(-3 ,3, -3, 3,-100,100);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    wHeight = height;
    wWidth = width;

    updateView(wHeight, wWidth);
}

void idle()
{
    glutPostRedisplay();
}

float alpha = 0, theta = pi / 2, distance = 8.0;
float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void cal_eye() {
    eye[0] = distance * cos(alpha) * cos(theta);
    eye[2] = distance * cos(alpha) * sin(theta);
    eye[1] = distance * sin(alpha);
}

void key(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:
        case 'q': {exit(0); break; }
        case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; }

        case ' ': {bAnim = !bAnim; break;}
        case 'o': {bWire = !bWire; break;}

        case 'a': {
            theta += 0.1;
            if (theta >= 2 * pi) {
                theta -= 2 * pi;
            }
            cal_eye();
            break;
        }
        case 'd': {
            theta -= 0.1;
            if (theta < 0) {
                theta += 2 * pi;
            }
            cal_eye();
            break;
        }
        case 'w': {
            alpha -= 0.1;
            if (alpha <= - pi / 2) {
                alpha += pi;
            }
            cal_eye();
            break;
        }
        case 's': {
            alpha += 0.1;
            if (alpha >= pi / 2) {
                alpha -= pi;
            }
            cal_eye();
            break;
        }
        case 'z': {
            if (distance < 30) {
                distance += 0.5;
            }
            cal_eye();
            break;
        }
        case 'c': {
            if (distance > 0.5) {
                distance -= 0.5;
            }
            cal_eye();
            break;
        }

        case 'l': {
            if (teapot_pos[0] < 1) {
                teapot_pos[0] += 0.1;
            }
            break;
        }
        case 'j': {
            if (teapot_pos[0] > -1) {
                teapot_pos[0] -= 0.1;
            }
            break;
        }
        case 'i': {
            if (teapot_pos[1] > -1) {
                teapot_pos[1] -= 0.1;
            }
            break;
        }
        case 'k': {
            if (teapot_pos[1] < 1) {
                teapot_pos[1] += 0.1;
            }
            break;
        }
        case 'e': {
            teapot_rotation = (teapot_rotation + 10) % 360;
            break;
        }
    }
}

void redraw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();									// Reset The Current Modelview Matrix

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
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_pos[] = {5,5,5,1};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glEnable(GL_LIGHT0);

//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    Draw_Scene();						// Draw Scene

    if (bAnim) fRotate    += 0.5f;

    //todo; hint: when you want to rotate the teapot, you may like to add another line here =)
    glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(480,480);
    int windowHandle = glutCreateWindow("Simple GLUT App");

    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

