#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

float fTranslate    = 0.0f;
float fRotate       = 0.0f;
float fScale        = 1.0f;

// draw a wired cuboid
void drawWiredCuboid(float x, float y, float z, float dx, float dy, float dz) {
    // switch to wire mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUAD_STRIP);

    glVertex3f(x, y, z);
    glVertex3f(x, y, z + dz);

    glVertex3f(x + dx, y, z);
    glVertex3f(x + dx, y, z + dz);

    glVertex3f(x + dx, y + dy, z);
    glVertex3f(x + dx, y + dy, z + dz);

    glVertex3f(x, y + dy, z);
    glVertex3f(x, y + dy, z + dz);

    glVertex3f(x, y, z);
    glVertex3f(x, y, z + dz);

    glEnd();
    // switch back to fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// draw a table with five cuboids
void drawTable() // This function draws a triangle with RGB colors
{
    drawWiredCuboid(-2.5, -2.5, 0, 5, 5, 1);
    // loop i in -1 and 1
    for (int i = -1; i < 2; i += 2) {
        // loop j in -1 and 1
        for (int j = -1; j < 2; j += 2) {
            // dx and dy may be negative, that's ok
            drawWiredCuboid(0.5 * i, 0.5 * j, 1, i, j, 3);
        }
    }
}

void reshape(int width, int height)
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    glViewport(0, 0, width, height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
    glutPostRedisplay();
}

void redraw()
{
    // If want display in wireframe mode
    //	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClear(GL_COLOR_BUFFER_BIT);
    // Reset The Current Modelview Matrix
    glLoadIdentity();

    glPushMatrix();
    // Place the triangle Left
    glTranslatef(-10.0f, 0.0f, -30.0f);
    // Translate in Y direction
    glTranslatef(0.0f, fTranslate, 0.0f);
    // Draw triangle
    drawTable();
    glPopMatrix();

    glPushMatrix();
    // Place the triangle at Center
    glTranslatef(0.0f, 0.0f, -30.0f);
    // Rotate around Y axis
    glRotatef(fRotate, 0, 1.0f, 0);
    // Draw triangle
    drawTable();
    glPopMatrix();


    glPushMatrix();
    // Place the triangle Right
    glTranslatef(10.0f, 0.0f, -30.0f);
    // Scale with the same value in x,y,z direction
    glScalef(fScale, fScale, fScale);
    // Draw triangle
    drawTable();
    glPopMatrix();

    fTranslate += 0.005f;
    fRotate    += 0.5f;
    fScale     -= 0.005f;

    if(fTranslate > 0.5f) fTranslate = 0.0f;
    if(fScale < 0.5f)     fScale     = 1.0f;
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    int windowHandle = glutCreateWindow("CG Lab2 Matrix");

    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}

