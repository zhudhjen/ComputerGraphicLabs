#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

std::map<int, int> windows;
const int main_window = 4;
int spin;

void idle() {
    spin++;
    if (spin > 360) {
        spin -= 360;
    }

    for (auto pair : windows) {
        glutSetWindow(pair.first);
        glutPostRedisplay();
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen

    glLoadIdentity();

    glColor3f(0.8, 0.8, 0.8);
    glTranslatef(0, 0, -20);
    glRotatef(spin, 1.0, 1.0, 1.0);
    glutWireCube(6);

    glutSwapBuffers();
}


void reshape(int w, int h) {
    if (h == 0) {
        h = 1;
    }

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    int window_id = windows.at(glutGetWindow());
    int row = window_id / 2;
    int col = window_id % 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (window_id != main_window) {
        glTranslatef(1 - col * 2, -1 + row * 2, 0);
        glScalef(2.0, 2.0, 1.0);
    }

    float aspect = w / h;
    gluPerspective(45, aspect, 1, 100);

    // print the projection matrix
    GLfloat m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    printf("Projection matrix of window %d", window_id);
    for (int i = 0; i < 16; ++i) {
        if (i % 4 == 0) {
            printf("\n");
        }
        printf("%12f", m[i]);
    }
    printf("\n\n");

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q':
        case 27:
            exit(0); // exit program when [ESC] key presseed
        default:
            break;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(300, 300);

    char title[100] = "Window 0";
    int window;
    for (int window_id = 0; window_id < 4; ++window_id) {
        int row = window_id / 2;
        int col = window_id % 2;
        glutInitWindowPosition(20 + 320 * col, 60 + 360 * row);
        window = glutCreateWindow(argv[0]);
        windows.insert(std::make_pair(window, window_id));
        title[7] = (char) (window_id + 48);
        glutSetWindowTitle(title);
        glutIdleFunc(idle);
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
    }

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(660, 60);
    window = glutCreateWindow(argv[0]);
    windows.insert(std::make_pair(window, main_window));
    glutSetWindowTitle("Main window");
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
