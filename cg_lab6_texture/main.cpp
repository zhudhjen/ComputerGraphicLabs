/*
 * @title   Texture
 * @author  Pitaya Zhu
 */

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef unsigned short Word;
typedef unsigned int DWord;
typedef int Long;
typedef unsigned char Byte;

#pragma pack(push, 1)
typedef struct {
    Word bfType;  //specifies the file type
    DWord bfSize;  //specifies the size in bytes of the bitmap file
    Word bfReserved1;  //reserved; must be 0
    Word bfReserved2;  //reserved; must be 0
    DWord bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BmpFileHeader;

typedef struct {
    DWord biSize;  //specifies the number of bytes required by the struct
    Long biWidth;  //specifies width in pixels
    Long biHeight;  //species height in pixels
    Word biPlanes; //specifies the number of color planes, must be 1
    Word biBitCount; //specifies the number of bit per pixel
    DWord biCompression;//spcifies the type of compression
    DWord biSizeImage;  //size of image in bytes
    Long biXPelsPerMeter;  //number of pixels per meter in x axis
    Long biYPelsPerMeter;  //number of pixels per meter in y axis
    DWord biClrUsed;  //number of colors used by th ebitmap
    DWord biClrImportant;  //number of colors that are important
} BmpInfoHeader;
#pragma pack(pop)

float fRotate;
const float pi = 3.14159;

bool bAnim = true;
bool bTeapotTexture = true;
bool bTeapotLighting = true;
bool bTableTexture = true;

int wHeight = 0;
int wWidth = 0;

float eye_alpha = 0.5, eye_theta = 0.5, eye_distance = 5.0;
float eye[] = {0, 0, 8, 1};
float center[] = {0, 0, 0, 1};

GLfloat light_pos[4] = {3, 3, 3, 1};
GLfloat light_color[4] = {1, 1, 1, 1};

GLuint texture_monet, texture_crack, texture_spot_crack, texture_tile;

void calcEyePos() {
    eye[0] = eye_distance * cosf(eye_alpha) * cosf(eye_theta);
    eye[1] = eye_distance * sinf(eye_alpha);
    eye[2] = eye_distance * cosf(eye_alpha) * sinf(eye_theta);
}

void drawBox(GLfloat size) {
    static GLfloat n[6][3] = {{-1.0,  0.0,  0.0},
                              { 0.0,  1.0,  0.0},
                              { 1.0,  0.0,  0.0},
                              { 0.0, -1.0,  0.0},
                              { 0.0,  0.0,  1.0},
                              { 0.0,  0.0, -1.0}};
    static GLint faces[6][4] = {{0, 1, 2, 3},
                                {3, 2, 6, 7},
                                {7, 6, 5, 4},
                                {4, 5, 1, 0},
                                {5, 6, 2, 1},
                                {7, 4, 0, 3}};
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(GL_QUADS);
//        glNormal3fv(&n[i][0]);

        glTexCoord2f(0, 0);
        glVertex3fv(&v[faces[i][0]][0]);

        glTexCoord2f(0, 1);
        glVertex3fv(&v[faces[i][1]][0]);

        glTexCoord2f(1, 1);
        glVertex3fv(&v[faces[i][2]][0]);

        glTexCoord2f(1, 0);
        glVertex3fv(&v[faces[i][3]][0]);

        glEnd();
    }
}

void drawLeg() {
    glScalef(1, 1, 3);
    drawBox(1.0);
}

void setMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat specular_level = 0) {
    GLfloat mat_diffuse[] = {r, g, b, 1};
    GLfloat mat_specular[] = {specular_level, specular_level, specular_level, 1};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
}

void drawScene() {

    glTranslatef(0, 0, -3);

    if (bTeapotLighting) {
        glEnable(GL_LIGHTING);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        glDisable(GL_LIGHTING);
    }

    // draw the teapot
    glEnable(GL_TEXTURE_2D);
    if (bTeapotTexture) {
        glBindTexture(GL_TEXTURE_2D, texture_monet);
    } else {
        glBindTexture(GL_TEXTURE_2D, texture_tile);
    }
    setMaterial(0.5, 0.7, 0.3, 1);
    glPushMatrix();
    glTranslatef(0, 0, 4 + 1);
    glRotatef(90, 1, 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    setMaterial(1, 1, 1);

    // draw the table face
    if (bTableTexture) {
        glBindTexture(GL_TEXTURE_2D, texture_spot_crack);
    } else {
        glBindTexture(GL_TEXTURE_2D, texture_crack);
    }
    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    drawBox(1.0);
    glPopMatrix();

    // draw legs
    glBindTexture(GL_TEXTURE_2D, texture_crack);

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

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
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
        case 'm': {
            bTeapotTexture = !bTeapotTexture;
            break;
        }
        case 'n': {
            bTableTexture = !bTableTexture;
            break;
        }
        case 'l': {
            bTeapotLighting = !bTeapotLighting;
            break;
        }
        default:
            break;
    }
}

void idle() {
    if (bAnim) {
        fRotate += 0.5f;
    }

    calcEyePos();
    glutPostRedisplay();
}

void redraw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHT0);

    glRotatef(fRotate, 0, 1.0f, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.4, 0.4, 0.4);
    drawScene();

    glutSwapBuffers();
}

Byte * loadBmpData(const char *filename, int &width, int &height) {
    Byte tmp;
    BmpFileHeader fh;
    BmpInfoHeader ih;
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("File not found\n");
        exit(1);
    }

    fread(&fh, sizeof(Byte), sizeof(BmpFileHeader), file);
    fread(&ih, sizeof(Byte), sizeof(BmpInfoHeader), file);

    width = ih.biWidth;
    height = ih.biHeight;

    Byte *matrix_img = (Byte *)calloc((size_t) (3 * height * width), sizeof(Byte));
    fseek(file, fh.bOffBits, SEEK_SET);
    fread(matrix_img, (size_t) (3 * width * height), sizeof(Byte), file);

    fclose(file);
    return matrix_img;
}

Byte * generateTextureData(int &width, int &height) {
    width = 256;
    height = 256;

    Byte *matrix_img = (Byte *)calloc((size_t) (3 * height * width), sizeof(Byte));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if ((i & 32) ^ (j & 32)) {
                matrix_img[3 * (i * width + j) + 0] = 0;
                matrix_img[3 * (i * width + j) + 1] = 0;
                matrix_img[3 * (i * width + j) + 2] = 0;
            } else {
                matrix_img[3 * (i * width + j) + 0] = 0;
                matrix_img[3 * (i * width + j) + 1] = 0;
                matrix_img[3 * (i * width + j) + 2] = 255;
            }
        }
    }

    return matrix_img;
}

GLuint loadTexture(const Byte *data, int width, int height) {
    GLuint texture_id;
    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    return texture_id;
}

void initTextures() {
    int width, height;
    Byte *data, *spot_data;

    data = loadBmpData("/Users/pitaya/Programming/Cpp/CG/cg_lab6_texture/Monet.bmp", width, height);
    texture_monet = loadTexture(data, width, height);

    data = loadBmpData("/Users/pitaya/Programming/Cpp/CG/cg_lab6_texture/Crack.bmp", width, height);
    texture_crack = loadTexture(data, width, height);

    spot_data = loadBmpData("/Users/pitaya/Programming/Cpp/CG/cg_lab6_texture/Spot.bmp", width, height);
    for (int i = 0; i < width * height * 3; ++i) {
        data[i] = (Byte) (data[i] * spot_data[i] / 255);
    }
    texture_spot_crack = loadTexture(data, width, height);

    data = generateTextureData(width, height);
    texture_tile = loadTexture(data, width, height);

    // set the BMP image upside down back
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
    glutInitWindowSize(480, 480);
    int windowHandle = glutCreateWindow("CG Lab 5 - Lighting and Material");

    initTextures();

    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}


