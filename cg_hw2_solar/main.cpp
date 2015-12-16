/*
 * @title   Teapot Move
 * @author  Pitaya Zhu
 */

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

const float PI = 3.141593;

bool b_animation = true;
bool b_wire = false;

int w_height = 0;
int w_width = 0;

GLfloat sun_color[4] = {0.6, 0.2, 0.2, 1.0};
GLfloat sun_radius = 2.0;
GLfloat sun_pos[3] = {0, 0, 0};

GLfloat earth_color[4] = {0.2, 0.2, 0.6, 1.0};
GLfloat earth_dist = 15.0;
GLfloat earth_radius = 1.0;
GLfloat earth_rot = 0;
GLfloat earth_rot_speed = 1.0;
GLfloat earth_pos[3] = {0, 0, 0};

GLfloat moon_color[4] = {0.5, 0.5, 0.5, 1.0};
GLfloat moon_dist = 3;
GLfloat moon_radius = 0.5;
GLfloat moon_rot = 0;
GLfloat moon_rot_speed = 5.0;
GLfloat moon_pos[3] = {0, 0, 0};
int moon_pos_history_count = 0;
GLfloat moon_pos_history[1024][2] = {};

void calcEye();

void drawAstroObj(GLfloat pos[3], GLfloat radius, GLfloat color[4]) {
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glColor3fv(color);
    glutSolidSphere(radius, 50, 50);
    glPopMatrix();
}

void drawOrbit(GLfloat pos[2], GLfloat radius) {
    int i;
    int line_count = 100; //# of triangles used to draw circle

    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < line_count; i++) {
        glVertex2f(pos[0] + radius * cosf(i * PI * 2 / line_count),
                   pos[1] + radius * sinf(i * PI * 2 / line_count));
    }
    glEnd();
}

void drawTrajectory() {
    glColor3f(0.2, 0.6, 0.2);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < moon_pos_history_count; ++i) {
        glVertex2fv(moon_pos_history[i]);
    }
    glEnd();
}

void updateView(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat aspect_ratio = (GLfloat) width / (GLfloat) height;

    gluPerspective(45.0f, aspect_ratio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    w_height = height;
    w_width = width;

    printf("width(%d), height(%d)\n", width, height);
    updateView(w_height, w_width);
}

void idle() {
    if (b_animation) {
        earth_rot += earth_rot_speed;
        if (earth_rot >= 360) {
            earth_rot -= 360;
        }
        earth_pos[0] = sun_pos[0] + earth_dist * cosf(PI * earth_rot / 180);
        earth_pos[1] = sun_pos[1] + earth_dist * sinf(PI * earth_rot / 180);

        moon_rot += moon_rot_speed;
        if (moon_rot >= 360) {
            moon_rot -= 360;
        }
        moon_pos_history[moon_pos_history_count][0] = moon_pos[0] = earth_pos[0] + moon_dist * cosf(PI * moon_rot / 180);
        moon_pos_history[moon_pos_history_count][1] = moon_pos[1] = earth_pos[1] + moon_dist * sinf(PI * moon_rot / 180);
        moon_pos_history_count = (moon_pos_history_count + 1) % 1024;
    }

    calcEye();

    glutPostRedisplay();
}

void viewText(std::string text) {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, w_width, 0, w_height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(10, 10);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

GLfloat alpha = 0.5, theta = PI / 2, distance = 50.0;
GLfloat eye[] = {0, 0, 0};
GLfloat center[] = {sun_pos[0], sun_pos[1], 0};

void calcEye() {
    eye[0] = center[0] + distance * cosf(alpha) * cosf(theta);
    eye[2] = center[2] + distance * cosf(alpha) * sinf(theta);
    eye[1] = center[1] + distance * sinf(alpha);
}

void key(unsigned char k, int x, int y) {
    switch (k) {
        case 27:
        case 'q': {
            exit(0);
            break;
        }
        case ' ': {
            b_animation = !b_animation;
            break;
        }
        case 'o': {
            b_wire = !b_wire;
            break;
        }

        case 'a': {
            theta += 0.05;
            if (theta >= 2 * PI) {
                theta -= 2 * PI;
            }
            break;
        }
        case 'd': {
            theta -= 0.05;
            if (theta < 0) {
                theta += 2 * PI;
            }
            break;
        }
        case 'w': {
            if (alpha - 0.1 >= -PI / 2) {
                alpha -= 0.02;
            }
            break;
        }
        case 's': {
            if (alpha + 0.1 <= PI / 2) {
                alpha += 0.02;
            }
            break;
        }
        case 'z': {
            if (distance < 60) {
                distance += 0.5;
            }
            break;
        }
        case 'c': {
            if (distance > 0.6) {
                distance -= 0.5;
            }
            break;
        }
        default:
            break;
    }
}

void redraw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);

    if (b_wire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnable(GL_DEPTH_TEST);

    glRotatef(-90, 1, 0, 0);

    drawAstroObj(sun_pos, sun_radius, sun_color);
    drawAstroObj(earth_pos, earth_radius, earth_color);
    drawAstroObj(moon_pos, moon_radius, moon_color);
    drawOrbit(sun_pos, earth_dist);
    drawOrbit(earth_pos, moon_dist);
    drawTrajectory();

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    int windowHandle = glutCreateWindow("CG Homework 2 - Solar System");

    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

