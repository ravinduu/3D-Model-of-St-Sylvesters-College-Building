#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <GL/glut.h>

using namespace std;

struct BitMapFile{
    int sizeX;
    int sizeY;
    unsigned char* data;
};

//variables of xzy axes
GLfloat xAxes = 20;
GLfloat yAxes = 20;
GLfloat zAxes = 20;

//variables to move Object Frame
GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

//variables to rotate Object Frame
GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

//variables to move the camera
GLfloat camX = 0.0f;
GLfloat camY = 0.0f;
GLfloat camZ = 0.0f;

//variables for show grid and axes
GLboolean showGrid = false;
GLboolean showAxes = false;

static unsigned int texture[4];

BitMapFile* getbmp(string filename) {
    int offset, headerSize;

    BitMapFile* bmpRGB = new BitMapFile;
    BitMapFile* bmpRGBA = new BitMapFile;

    ifstream infile(filename.c_str(), ios::binary);

    infile.seekg(10);
    infile.read((char*)&offset, 4);

    infile.read((char*)&headerSize, 4);

    infile.seekg(18);
    infile.read((char*)&bmpRGB->sizeX, 4);
    infile.read((char*)&bmpRGB->sizeY, 4);

    int padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;
    int sizeScanline = 3 * bmpRGB->sizeX + padding;
    int sizeStorage = sizeScanline * bmpRGB->sizeY;
    bmpRGB->data = new unsigned char[sizeStorage];

    infile.seekg(offset);
    infile.read((char*)bmpRGB->data, sizeStorage);

    int startScanline, endScanlineImageData, temp;
    for (int y = 0; y < bmpRGB->sizeY; y++)
    {
        startScanline = y * sizeScanline;
        endScanlineImageData = startScanline + 3 * bmpRGB->sizeX;
        for (int x = startScanline; x < endScanlineImageData; x += 3)
        {
            temp = bmpRGB->data[x];
            bmpRGB->data[x] = bmpRGB->data[x + 2];
            bmpRGB->data[x + 2] = temp;
        }
    }

    bmpRGBA->sizeX = bmpRGB->sizeX;
    bmpRGBA->sizeY = bmpRGB->sizeY;
    bmpRGBA->data = new unsigned char[4 * bmpRGB->sizeX * bmpRGB->sizeY];

    for (int j = 0; j < 4 * bmpRGB->sizeY * bmpRGB->sizeX; j += 4)
    {
        bmpRGBA->data[j] = bmpRGB->data[(j / 4) * 3];
        bmpRGBA->data[j + 1] = bmpRGB->data[(j / 4) * 3 + 1];
        bmpRGBA->data[j + 2] = bmpRGB->data[(j / 4) * 3 + 2];
        bmpRGBA->data[j + 3] = 0xFF;
    }
    return bmpRGBA;
}

void loadExternalTextures() {
    BitMapFile* image[4];
    image[0] = getbmp("textures/roof3.bmp");
    image[1] = getbmp("textures/roof4.bmp");
    image[2] = getbmp("textures/floor.bmp");
    image[3] = getbmp("textures/grass.bmp");

    for (GLint i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i]->sizeX, image[i]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}


void initLighting() {
    GLfloat L0_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat L0_Diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat L0_Specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat L0_postion[] = { 200, 200, 200, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, L0_Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, L0_Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, L0_Specular);
    glLightfv(GL_LIGHT0, GL_POSITION, L0_postion);

    GLfloat specularReflectance[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
    glMateriali(GL_FRONT, GL_SHININESS, 50);
}


void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GLfloat globalAmbient[] = { 0.4, 0.4, 0.4, 1.0 };
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -3.0, 3.0);
    
    glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glGenTextures(3, texture);
    loadExternalTextures();

    initLighting();
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
}


//draw xyz axes
void drawAxes() {
    glPushMatrix();
    glBegin(GL_LINES);

    glLineWidth(1.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-xAxes, 0, 0);
    glVertex3f(xAxes, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -yAxes, 0);
    glVertex3f(0, yAxes, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -zAxes);
    glVertex3f(0, 0, -zAxes);

    glEnd();
    glPopMatrix();
}


//draw th grid in x and z axis
void drawGrid() {
    glPushMatrix();
    
    glColor3f(1.0, 1.0, 1.0);

    GLfloat ext = xAxes;
    GLfloat step = 1.0f;
    GLfloat yGrid = 0.0f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();
    glPopMatrix();
}


void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        moveZ += 1;
    if (key == GLUT_KEY_DOWN)
        moveZ -= 1;
    if (key == GLUT_KEY_LEFT)
        rotY -= 5.0;
    if (key == GLUT_KEY_RIGHT)
        rotY += 1.0;
    if (key == GLUT_KEY_F2)
        showAxes = !showAxes;
    if (key == GLUT_KEY_F3)
        showGrid = !showGrid;

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 'w')
        camY += 1;
    if (key == 's')
        camY -= 1;

    if (key == 'c')
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == 'C')
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (key == 'k')
        glDisable(GL_LIGHT0);
    if (key == 'K')
        glEnable(GL_LIGHT0);

    glutPostRedisplay();
}


void drawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat length, GLfloat r, GLfloat b, GLfloat g) {

    // WALL FRONT
    glBegin(GL_QUADS);
    if (r == 0 && g == 0 && b == 0) {
        glColor3f(1.000, 0.922, 0.804);
    }
    else {
        glColor3f(r, g, b);
    }
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x, y, z);
    glVertex3f(x, y - height , z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width, y, z);
    glEnd();

    // WALL BACK
    glBegin(GL_QUADS);
    if (r == 0 && g == 0 && b == 0) {
        glColor3f(1.000, 0.871, 0.678);
    }
    else {
        glColor3f(r, g, b);
    }
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x, y, z - length);
    glVertex3f(x, y - height, z - length);
    glVertex3f(x + width, y - height, z - length);
    glVertex3f(x + width, y, z - length);
    glEnd();

    // WALL LEFT
    glBegin(GL_QUADS);
    if (r == 0 && g == 0 && b == 0) {
        glColor3f(0.871, 0.722, 0.529);
    }
    else {
        glColor3f(r, g, b);
    }
    glVertex3f(x, y, z);
    glVertex3f(x, y - height, z);
    glVertex3f(x, y - height, z - length);
    glVertex3f(x, y, z - length);
    glEnd();

    // WALL RIGHT
    glBegin(GL_QUADS);
    if (r == 0 && g == 0 && b == 0) {
        glColor3f(0.957, 0.643, 0.376);
    }
    else {
        glColor3f(r, g, b);
    }
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width, y - height, z - length);
    glVertex3f(x + width, y, z - length);
    glEnd();

    // WALL TOP
    glBegin(GL_QUADS);
    if (r == 0 && g == 0 && b == 0) {
        glColor3f(0.722, 0.525, 0.043);
    }
    else {
        glColor3f(r, g, b);
    }
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x, y, z);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y, z - length);
    glVertex3f(x, y, z - length);
    glEnd();

    //WALL BOTTOM
    glBegin(GL_QUADS);
    if (r != 0 && g != 0 && b != 0) {
        glColor3f(r, g, b);
    }
    else {
        glColor3f(0.824, 0.412, 0.118);
    }
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(x, y - height, z);
    glVertex3f(x + width, y - height , z);
    glVertex3f(x + width, y - height, z - length);
    glVertex3f(x, y - height, z - length);
    glEnd();
}

void drawHexagonalPrism(GLfloat x[], GLfloat z[], GLfloat y, GLfloat h){
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    for (int i1 = 0; i1 < 6; ++i1){
        int i2 = (i1 + 1) % 6;
        glVertex3f(x[i1], y, z[i1]);
        glVertex3f(x[i2], y, z[i2]);
        glVertex3f(x[i2], y + h, z[i2]);
        glVertex3f(x[i1], y + h, z[i1]);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    for (int i = 0; i < 6; ++i)
        glVertex3f(x[i], y, z[i]);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    for (int i = 0; i < 6; ++i)
        glVertex3f(x[i], y + h, z[i]);
    glEnd();
}

void drawRightBuilding() {
    glPushMatrix();

    //columns left to right
    drawCube(15, 5, 6, 4, 5, 0.5, 0 , 0, 0);

    drawCube(15, 5, 4.5, 4, 5, 0.5, 0 , 0, 0);
    drawCube(15, 5.5, 4, 4, 5.5, 0.5, 0 , 0, 0);

    drawCube(15, 5.5, 2.5, 4, 5.5, 0.5, 0 , 0, 0);
    drawCube(15, 6, 2, 4, 6, 0.5, 0 , 0, 0);

    drawCube(15, 6, 0, 4, 6, 1.0, 0 , 0, 0); //center

    drawCube(15, 6, -2, 4, 6, 0.5, 0 , 0, 0);
    drawCube(15, 5.5, -2.5, 4, 5.5, 0.5, 0 , 0, 0);

    drawCube(15, 5, -4.5, 4, 5, 0.5, 0 , 0, 0);
    drawCube(15, 5.5, -4, 4, 5.5, 0.5, 0 , 0, 0);

    drawCube(15, 5, -6, 4, 5, 0.5, 0 , 0, 0);

    //rows top to bottom
    drawCube(15, 6, -2, 4, 0.5, -4, 0 , 0, 0); //top two rows
    drawCube(15, 5.5, -4, 4, 0.5, -8, 0 , 0, 0);

    for (GLfloat y = 5; y >= 0.5; y -= 1.5, 0 , 0, 0) {
        drawCube(15, y, 6, 4, 0.5, 12, 0 , 0, 0);

    }
    //drawCube(5, 3.5, -6, 4, 0.5, 12);
    //drawCube(5, 2, -6, 4, 0.5, 12);
    //drawCube(5, 0.5, -6, 4, 0.5, 12);

    //window rows top to bottom
    glPushMatrix();

    drawCube(15.2, 4.5, 6, 3.6, 1, 12, 0.412, 0.412, 0.412);
    for (GLfloat y = 3; y >= 1.5; y -= 1.5) {
        drawCube(15, y, 6, 3.8, 1, 12, 0.412, 0.412, 0.412);

    }
    glPopMatrix();

    //drawCube(5.8, 3, -6, 3, 1, 12);
    //drawCube(5.8, 1.5, -6, 3, 1, 12);

    glPopMatrix();
}

void drawRightSideCornerBuilding() {
    glPushMatrix();

    //4 corner beams (columns)
    drawCube(15, 7, 10, 1, 7, 1 , 0 , 0, 0);
    drawCube(18, 7, 10, 1, 7, 1 , 0 , 0, 0);
    drawCube(18, 7, 7, 1, 7, 1 , 0 , 0, 0);
    drawCube(15, 7, 7, 1, 7, 1 , 0 , 0, 0);

    //2 small beams in front and left side (columns)

    drawCube(16, 7, 9.8, 0.5, 7, 0.8, 0 , 0, 0);//front
    drawCube(17.5, 7, 9.8, 0.5, 7, 0.8, 0 , 0, 0);
    drawCube(18, 7, 9, 0.8, 7, 0.5, 0 , 0, 0);//right
    drawCube(18, 7, 7.5, 0.8, 7, 0.5, 0 , 0, 0);
   

    //wall rows
    //right side (top to bottom)
    drawCube(18, 7, 8.5, 0.8, 0.5, 1, 0 , 0, 0);
    drawCube(18, 5.5, 8.5, 0.8, 1, 1, 0 , 0, 0);
    drawCube(18, 3.5, 8.5, 0.8, 1, 1, 0 , 0, 0);
    drawCube(18, 0.5, 8.5, 0.8, 0.5, 1, 0 , 0, 0);

    //front side (top to bottom)
    drawCube(16.5, 7, 9.8, 1, 0.5, 0.8, 0 , 0, 0);
    drawCube(16.5, 5.5, 9.8, 1, 1, 0.8, 0 , 0, 0);
    drawCube(16.5, 3.5, 9.8, 1, 1, 0.8, 0 , 0, 0);
    drawCube(16.5, 0.5, 9.8, 1, 0.5, 0.8, 0 , 0, 0);

    glPopMatrix();

    //small balcony parts in middle
    glPushMatrix();
    drawCube(15.1, 5.1, 9.9, 3.899, 0.2, 3.899, 0.502, 0.000, 0.000);
    drawCube(15.1, 3.1, 9.9, 3.899, 0.2, 3.899, 0.502, 0.000, 0.000);
    glPopMatrix();

    //core
    glPushMatrix();
    drawCube(15.1, 7, 9.5, 3.4, 7, 3.4, 0.412, 0.412, 0.412);
    glPopMatrix();


    //draw the hexagon part in the top
    GLfloat x[] = { 16.134f, 16.134f, 17.0f, 17.866f, 17.866f, 17.0f };
    GLfloat z[] = { 7.5f, 8.5f, 9.0f, 8.5f, 7.5f, 7.0f };
    GLfloat y = 7.0f;
    GLfloat h = 1.0f;


    //draw top balcony
    glPushMatrix();
    glColor3f(0.502, 0.000, 0.000);
    drawCube(14.9, 7.1, 10.2, 4.2, 0.1, 4.3, 0.502, 0.000, 0.000);
    drawCube(15.2, 7.4, 9.8, 3.6, 0.4, 0.2, 0.502, 0.000, 0.000);
    drawCube(15.2, 7.4, 9.8, 0.2, 0.4, 3.6, 0.502, 0.000, 0.000);
    drawCube(15.2, 7.4, 6.2, 3.6, 0.4, 0.2, 0.502, 0.000, 0.000);
    drawCube(18.6, 7.4, 9.8, 0.2, 0.4, 3.6, 0.502, 0.000, 0.000);
    glPopMatrix();

    //draw hexagon building on top
    glPushMatrix();
    glColor3f(0.961, 0.871, 0.702);
    drawHexagonalPrism(x, z, y, h);
    glPopMatrix();

    //cone roof part on the hexagon
    glPushMatrix();
    glColor3f(0.412, 0.412, 0.412);
    glTranslatef(17, 8, 8);
    glRotatef(90, -1.0, 0, 0);
    glutSolidCone(0.5, 1.3, 6, 3);
    glutSolidCone(1.3, 1, 6, 3);
    glPopMatrix();

}

void drawMiddleRightSideBuilding(){
    glPushMatrix();
    //columns
    drawCube(5, 7, 10, 1.5, 7, 4, 0, 0, 0);
    for (GLfloat x = 8; x <= 12; x += 2) {
        drawCube(x, 7, 10, 1, 7, 4, 0, 0, 0);
    }
    drawCube(14, 7, 10, 1, 7, 4, 0, 0, 0);

    //rows
    drawCube(5.5, 7, 10, 9.5, 2, 4, 0, 0, 0);
    drawCube(5.5, 3.5, 10, 9.5, 1, 4, 0, 0, 0);
    drawCube(5.5, 0.5, 10, 9.5, 0.5, 4, 0, 0, 0);

    //windows
    drawCube(5.501, 5, 9.8, 9.498, 1.5, 3.6, 0.412, 0.412, 0.412);
    drawCube(5.501, 2.5, 9.8, 9.498, 2, 3.6, 0.412, 0.412, 0.412);

    //small line strips
    drawCube(5.001, 6.1, 10.2, 9.998, 0.2, 0.2, 0.502, 0.000, 0.000);
    drawCube(5.001, 5.5, 10.2, 9.998, 0.2, 0.2, 0.502, 0.000, 0.000);
    drawCube(5.001, 3.1, 10.2, 9.998, 0.2, 0.2, 0.502, 0.000, 0.000);

    glPopMatrix();
}

void drawFrontMiddleBuilding() {
    glPushMatrix();

    //wall columns
    drawCube(-7, 5, 10, 1, 5, 4, 0, 0.000, 0.000);//front center
    drawCube(-5, 5, 10, 0.5, 2.5, 3.5, 0, 0.000, 0.000);
    for (GLfloat x = -3.5; x <= 4.5; x += 2) {
        GLfloat w = x == 0.5 ? 3.5 : 4;
        drawCube(x, 5, 10, 0.5, 5, w, 0, 0.000, 0.000);
    }

    //wall rows
    drawCube(-7, 5, 10, 12, 0.5, 4, 0, 0.000, 0.000);
    drawCube(-7, 3.5, 10, 12, 1, 4, 0, 0.000, 0.000);
    drawCube(-3.5, 0.5, 10, 8, 0.5, 4, 0, 0.000, 0.000);

    //windows
    drawCube(-6.99, 4.5, 9.8, 11.899, 1, 3.5, 0.412, 0.412, 0.412);
    drawCube(-6.99, 4.5, 9.8, 11.899, 1, 3.5, 0.412, 0.412, 0.412);
    drawCube(-3.49, 2.5, 9.8, 8.399, 2, 3.5, 0.412, 0.412, 0.412);

    //small line
    drawCube(-7, 3.1, 10.2, 12, 0.2, 0.2, 0.502, 0.000, 0.000);

    glPopMatrix();
}


void drawFrontLeftBuilding() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    //columns
    drawCube(-9, 5, 10, 0.5, 5, 4, 0, 0.000, 0.000);
    drawCube(-11, 5, 10, 0.5, 5, 4, 0, 0.000, 0.000);

    //wall rows
    drawCube(-11, 5, 10, 5, 0.5, 4, 0, 0.000, 0.000);
    drawCube(-11, 3.5, 10, 5, 1, 4, 0, 0.000, 0.000);
    drawCube(-11, 0.5, 10, 5, 0.5, 4, 0, 0.000, 0.000);

    //small line
    drawCube(-11, 3.1, 10.2, 5, 0.2, 0.2, 0.502, 0.000, 0.000);

    //windows
    drawCube(-10.99, 4.5, 9.8, 3.899, 1, 3.5, 0.412, 0.412, 0.412);
    drawCube(-10.99, 4.5, 9.8, 3.899, 1, 3.5, 0.412, 0.412, 0.412);
    drawCube(-10.49, 2.5, 9.8, 3.899, 2, 3.5, 0.412, 0.412, 0.412);

    //wall side traingle part
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glColor3f(0.871, 0.722, 0.529);
    glVertex3f(-11, 5, 10);
    glVertex3f(-11, 6, 8);
    glVertex3f(-11, 5, 6);
    glEnd();

    //roof
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-11, 4.75, 10.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-11, 6, 8);
    glTexCoord2f(1.0, 1.0); glVertex3f(5, 6, 8);
    glTexCoord2f(1.0, 0.0); glVertex3f(5, 4.75, 10.5);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-11, 6, 8);
    glTexCoord2f(0.0, 1.0); glVertex3f(-11, 4, 4);
    glTexCoord2f(1.0, 1.0); glVertex3f(5, 4, 4);
    glTexCoord2f(1.0, 0.0); glVertex3f(5, 6, 8);
    glEnd();
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawLefttBuilding() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    for (GLint i = 0; i > -18; i -= 2) {
        glPushMatrix();
        glTranslatef(0.0, 0.0, i);
        drawCube(-11, 5, 6, 4, 5, 0.5, 0, 0, 0);//right
        glPopMatrix();
    }

    //wall rows
    drawCube(-11, 5, 6, 4, 0.5, 16, 0, 0.000, 0.000);
    drawCube(-11, 3.5, 6, 4, 1, 16, 0, 0.000, 0.000);
    drawCube(-11, 0.5, 6, 4, 0.5, 16, 0, 0.000, 0.000);

    //small line
    drawCube(-11.2, 3.1, 10.2, 0.2, 0.2, 20.2, 0.502, 0.000, 0.000);

    //windows
    drawCube(-10.9, 4.5, 5.8, 3.899, 1, 15.899, 0.412, 0.412, 0.412);
    drawCube(-10.9, 2.5, 5.8, 3.899, 2, 15.899, 0.412, 0.412, 0.412);
    
    //roof
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glNormal3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);  glVertex3f(-11, 5, 6);
    glTexCoord2f(0.0, 1.0); glVertex3f(-9.5, 6, 8.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-9.5, 6, -8.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-11, 5, -7);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-9.5, 6, 8.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-5, 4.75, 6);
    glTexCoord2f(1.0, 1.0); glVertex3f(-5, 4.75, -7);
    glTexCoord2f(1.0, 0.0); glVertex3f(-9.5, 6, -8.5);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void drawBackBuilding() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    for (GLint i = 0; i < 28; i += 2) {
        glPushMatrix();
        glTranslatef(i, 0.0, 0.0);
        drawCube(-10, 5, -6.5, 0.5, 5, 4, 0, 0, 0);//back
        glPopMatrix();
    }

    //wall rows
    drawCube(-10, 5, -6.5, 26, 0.5, 4, 0, 0.000, 0.000);
    drawCube(-10, 3.5, -6.5, 26, 1, 4, 0, 0.000, 0.000);
    drawCube(-10, 0.5, -6.5, 26, 0.5, 4, 0, 0.000, 0.000);

    //windows
    drawCube(-10.9, 4.5, -6.6, 27, 1, 3.899, 0.412, 0.412, 0.412);
    drawCube(-10.9, 2.5, -6.6, 27, 2, 3.899, 0.412, 0.412, 0.412);

    //roof
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-11, 4.5, -11.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-11, 6, -8.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(18, 6, -8.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(18, 4.5, -11.5);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-11, 6, -8.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-11, 4, -4);
    glTexCoord2f(1.0, 1.0); glVertex3f(18, 4, -4);
    glTexCoord2f(1.0, 0.0); glVertex3f(18, 6, -8.5);
    glEnd();
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawInnerRightMiddleBuilding() {
    glPushMatrix();
    //inner right middle
    //12.5
    for (GLint i = 0; i > -13; i -= 4) {
        glPushMatrix();
        glTranslatef(0.0, 0.0, i);
        drawCube(9, 3, 6, 6, 3, 2, 0, 0, 0);
        glPopMatrix();
    }

    //wall rows
    drawCube(9, 3.5, 6, 6, 1, 12.5, 0, 0.000, 0.000);
    drawCube(9, 0.5, 6, 6, 0.5, 12.5, 0, 0.000, 0.000);

    //windows
    drawCube(9.2, 2.5, 6, 5.899, 2, 12.5, 0.412, 0.412, 0.412);
    glPopMatrix();
}

void drawBalcony() {
    glPushMatrix();
    //balcony
    for (GLint i = 0; i < 12; i += 2) {
        glPushMatrix();
        glTranslatef(i, 0.0, 0.0);
        drawCube(-5, 3.1, 4.2, 0.1, -0.95, 0.1, 0.1, 0.0, 0.1);
        glPopMatrix();
    }
    drawCube(-5, 3.6, 4.2, 10, 0.2, 0.2, 0.100, 0.100, 0.000);
    drawCube(-7, 3.1, 6, 16, 0.2, 2, 0.502, 0.100, 0.000);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, -10.0);
    for (GLint i = 0; i < 14; i += 2) {
        glPushMatrix();
        glTranslatef(i, 0.0, 0.0);
        drawCube(-5, 3.1, 6, 0.1, -0.95, 0.1, 0.1, 0.0, 0.1);
        glPopMatrix();
    }
    drawCube(-5, 3.6, 6, 12, 0.2, 0.2, 0.100, 0.100, 0.000);
    drawCube(-7, 3.1, 6, 16, 0.2, 2, 0.502, 0.100, 0.000);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-11.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    for (GLint i = 2; i < 10; i += 2) {
        glPushMatrix();
        glTranslatef(i, 0.0, 0.0);
        drawCube(-5, 3.1, 6, 0.1, -1.6, 0.1, 0.1, 0.0, 0.1);
        glPopMatrix();
    }

    drawCube(-4, 3.6, 6, 8, 0.2, 0.2, 0.100, 0.100, 0.000);
    drawCube(-7, 3.1, 6, 16, 0.2, 2, 0.502, 0.100, 0.000);
    glPopMatrix();
}

void drawStairs(){
    glPushMatrix();
    glTranslatef(3.0, 0.0, 4.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    for (GLint i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(0.0, i, i);
        drawCube(0.0, 0.0, 0.0, 2.0, 0.2, 1.0, 0.0, 0.0, 0.0);
        glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(0.0, 3, 6);
    drawCube(0.0, 0.0, 0.0, 2.0, 0.2, 3.0, 0.0, 0.0, 0.0);
    glPopMatrix();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Depth Buffer should be cleared everytime a redraw happens
    
    glPushMatrix();

    //camera position
    gluLookAt(0.0 + camX, 20.0 + camY, 30.0 + camZ, 0, 0, 0, 0, 1.0, 0);

    // move the object frame using keyboard keys
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

    drawRightBuilding();
    drawRightSideCornerBuilding();
    drawMiddleRightSideBuilding();
    drawFrontMiddleBuilding();
    drawFrontLeftBuilding();
    drawLefttBuilding();
    drawBackBuilding();
    drawInnerRightMiddleBuilding();
    drawBalcony();
    drawStairs();

    //floor
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0); glVertex3f(-12, 0, 13);
    glTexCoord2f(0.0, 1.0); glVertex3f(-12, 0, -12);
    glTexCoord2f(1.0, 1.0); glVertex3f(20, 0, -12);
    glTexCoord2f(1.0, 0.0); glVertex3f(20, 0, 13);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawCube(-12, -0.0009, 13, 32, 0.2, 25, 0, 0, 0);

    //courtyard
    glPushMatrix();
    glTranslatef(-5, 0, 5);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.3, 0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0, 0.3, -10);
    glTexCoord2f(1.0, 1.0); glVertex3f(5, 0.3, -10);
    glTexCoord2f(1.0, 0.0); glVertex3f(5, 0.3, 0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    drawCube(0, 0.209, 0, 5, 0.3, 10, 0.0, 0, 1.0);
    glPopMatrix();

    if(showGrid)
        drawGrid(); //draw the grids
    if(showAxes)
        drawAxes(); //draw the xyz axes

    glPopMatrix();                 
    glutSwapBuffers();
}

void changeSize(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, aspect_ratio, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1500, 1500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("St. Sylvester's College Building");

    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);    
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    init();
    glutMainLoop();

    return 0;
}