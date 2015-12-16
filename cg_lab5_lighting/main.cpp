/*
 * @title   Lighting and Material test
 * @author  Pitaya Zhu
 */

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float fRotate;
const float pi = 3.14159;

bool bAnim = true;
bool light_mode = true;

int wHeight = 0;
int wWidth = 0;

float eye_alpha = 0.5, eye_theta = 0.5, eye_distance = 5.0;
float eye[] = {0, 0, 8, 1};
float center[] = {0, 0, 0, 1};

float light_alpha = 0.8, light_theta = 1.0, light_distance = 2.0;
GLfloat light_pos[] = {0, 0, 0, 1};

int light_color_id = 0;
GLfloat light_color[4] = {1, 1, 1, 1};

GLfloat white[] = {1.0, 1.0, 1.0, 1.0};

GLfloat spot_pos[] = {0, 3, 0, 1};
GLfloat spot_dir[] = {0, -10, 0, 1};
GLfloat spot_cutoff = 10;

void calcEyePos() {
    eye[0] = eye_distance * cosf(eye_alpha) * cosf(eye_theta);
    eye[1] = eye_distance * sinf(eye_alpha);
    eye[2] = eye_distance * cosf(eye_alpha) * sinf(eye_theta);
}

void calcLightPos() {
    light_pos[0] = light_distance * cosf(light_alpha) * cosf(light_theta);
    light_pos[1] = light_distance * sinf(light_alpha);
    light_pos[2] = light_distance * cosf(light_alpha) * sinf(light_theta);
}

void calcLightColor() {
    light_color[0] = 1 - (light_color_id & 1) / 1;
    light_color[1] = 1 - (light_color_id & 2) / 2;
    light_color[2] = 1 - (light_color_id & 4) / 4;
}

void drawLeg() {
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}

void setMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat specular_level = 0) {
    GLfloat mat_diffuse[] = {r, g, b, 1};
    GLfloat mat_specular[] = {specular_level, specular_level, specular_level, 1};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
}

void drawScene() {
    // draw the teapot
    setMaterial(0.85, 0.65, 0.2, 0.7);
    glPushMatrix();
    glTranslatef(0, 0, 4 + 1);
    glRotatef(90, 1, 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();

    // draw the table face
    setMaterial(1, 0, 0);
    int x_divide = 100;
    int y_divide = 80;
    double x_part = 5.0 / x_divide;
    double y_part = 5.0 / x_divide;

    glPushMatrix();
    glTranslatef(-2.5, -2, 3.5);
    glScalef(x_part, y_part, 1);
    for (int i = 0; i < x_divide; ++i) {
        for (int j = 0; j < y_divide; ++j) {
            glutSolidCube(1.0);
            glTranslatef(0, 1, 0);
        }
        glTranslatef(1, -y_divide, 0);
    }
    glPopMatrix();

    // draw the test sphere
    setMaterial(1, 1, 1);
    glPushMatrix();
    glTranslatef(0, 0, 0.5);
    glutSolidSphere(0.5, 100, 100);
    glPopMatrix();

    // draw legs
    setMaterial(0, 1, 0);
    glPushMatrix();
    glTranslatef(1.5, 1, 1.5);
    drawLeg();
    glPopMatrix();

    setMaterial(1, 1, 0);
    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    drawLeg();
    glPopMatrix();

    setMaterial(0, 1, 1);
    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    drawLeg();
    glPopMatrix();

    setMaterial(0, 0, 1);
    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    drawLeg();
    glPopMatrix();

}

void drawLightSource() {
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glColor3f(light_color[0], light_color[1], light_color[2]);
    if (light_mode) {
        glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
        glutWireSphere(0.1, 10, 10);
    } else {
        float height = spot_pos[1];
        glTranslatef(0, spot_pos[1], 0);
        glRotatef(atanf(sqrtf(spot_dir[0] * spot_dir[0] + spot_dir[2] * spot_dir[2]) / spot_dir[1]) / pi * 180,
                  spot_dir[2], 0, -spot_dir[0]);
//        glTranslatef(spot_pos[0], spot_pos[1], spot_pos[2]);
        glTranslatef(0, -spot_pos[1], 0);
        glRotatef(-90, 1, 0, 0);

        glutWireCone(height * tanf(spot_cutoff * pi / 180), height, 16, 1);
    }
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void updateView(int width, int height) {
    glViewport(0, 0, width, height);                        // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

    float aspect_ratio = (GLfloat) width / (GLfloat) height;

    gluPerspective(45.0f, aspect_ratio, 1.0f, 20.0f);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
}

void reshape(int width, int height) {
    if (height == 0)                                        // Prevent A Divide By Zero By
    {
        height = 1;                                        // Making Height Equal One
    }

    wWidth = width;
    wHeight = height;

    updateView(wWidth, wHeight);
}

void key(unsigned char k, int x, int y) {
    switch (k) {
        case 27:
        case 'q': {
            exit(0);
        }
        case ' ': {
            bAnim = !bAnim;
            break;
        }
        case 'a': {
            eye_theta += 0.1;
            if (eye_theta >= 2 * pi) {
                eye_theta -= 2 * pi;
            }
            break;
        }
        case 'd': {
            eye_theta -= 0.1;
            if (eye_theta < 0) {
                eye_theta += 2 * pi;
            }
            break;
        }
        case 'w': {
            if (eye_alpha - 0.1 >= -pi / 2) {
                eye_alpha -= 0.1;
            }
            break;
        }
        case 's': {
            if (eye_alpha + 0.1 <= pi / 2) {
                eye_alpha += 0.1;
            }
            break;
        }
        case 'z': {
            if (eye_distance < 20) {
                eye_distance += 0.5;
            }
            break;
        }
        case 'c': {
            if (eye_distance > 0.5) {
                eye_distance -= 0.5;
            }
            break;
        }

        case 'j': {
            light_theta += 0.1;
            if (light_theta >= 2 * pi) {
                light_theta -= 2 * pi;
            }
            break;
        }
        case 'l': {
            light_theta -= 0.1;
            if (light_theta < 0) {
                light_theta += 2 * pi;
            }
            break;
        }
        case 'i': {
            if (light_alpha - 0.1 >= -pi / 2) {
                light_alpha -= 0.1;
            }
            break;
        }
        case 'k': {
            if (light_alpha + 0.1 <= pi / 2) {
                light_alpha += 0.1;
            }
            break;
        }
        case 'u': {
            if (light_distance < 20) {
                light_distance += 0.5;
            }
            break;
        }
        case 'o': {
            if (light_distance > 0.5) {
                light_distance -= 0.5;
            }
            break;
        }
        case 'n': {
            light_mode = !light_mode;
            break;
        }
        case 'm': {
            light_color_id = (light_color_id + 1) % 7;
            break;
        }
        case 'p': {
            printf("%f %f %f", light_color[0], light_color[1], light_color[2]);
            break;
        }
        case 't':
            if (spot_dir[2] < 10) {
                spot_dir[2] += 0.5;
            }
            break;
        case 'g':
            if (spot_dir[2] > -10) {
                spot_dir[2] -= 0.5;
            }
            break;
        case 'f':
            if (spot_dir[0] < 10) {
                spot_dir[0] += 0.5;
            }
            break;
        case 'h':
            if (spot_dir[0] > -10) {
                spot_dir[0] -= 0.5;
            }
            break;
        case 'r':
            if (spot_cutoff > 1) {
                spot_cutoff -= 0.5;
            }
            break;
        case 'y':
            if (spot_cutoff < 180) {
                spot_cutoff += 0.5;
            }
            break;
        default:
            break;
    }
}

void idle() {
    if (bAnim) {
        fRotate += 0.5f;
        light_theta -= 0.04f;
    }

    calcEyePos();
    calcLightPos();
    calcLightColor();
    glutPostRedisplay();
}

void redraw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();                                    // Reset The Current Modelview Matrix

    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);

    if (light_mode) {
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
    } else {
        glLightfv(GL_LIGHT0, GL_POSITION, spot_pos);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);             //裁减角度
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);  //光源方向
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2);

    }

    glEnable(GL_LIGHT0);

    drawLightSource();

    glRotatef(fRotate, 0, 1.0f, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    drawScene();

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
    glutInitWindowSize(480, 480);
    int windowHandle = glutCreateWindow("CG Lab 5 - Lighting and Material");

    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

