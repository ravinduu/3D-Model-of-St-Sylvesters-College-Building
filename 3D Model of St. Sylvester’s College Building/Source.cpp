#include <iostream>
#include <GL/glut.h>


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


void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);                // Enable the Depth testig in openGL
    
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

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 'w')
        camY += 0.5;
    if (key == 's')
        camY -= 0.5;

    if (key == 'c')
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == 'C')
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glutPostRedisplay();

}


void drawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat length) {
    
    // WALL FRONT
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(x, y, z);
    glVertex3f(x, y - height , z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width, y, z);
    glEnd();

    // WALL BACK
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.3f, 0.5f);
    glVertex3f(x, y, z - length);
    glVertex3f(x, y - height, z - length);
    glVertex3f(x + width, y - height, z - length);
    glVertex3f(x + width, y, z - length);
    glEnd();

    // WALL LEFT
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(x , y, z);
    glVertex3f(x, y - height, z);
    glVertex3f(x, y - height, z - length);
    glVertex3f(x, y, z - length);
    glEnd();

    // WALL RIGHT
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width, y - height, z - length);
    glVertex3f(x + width, y, z - length);
    glEnd();

    // WALL TOP
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(x, y, z);
    glVertex3f(x + width, y , z);
    glVertex3f(x + width, y, z - length);
    glVertex3f(x, y, z - length);
    glEnd();

    //WALL BOTTOM
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(x, y - height, z);
    glVertex3f(x + width, y - height , z);
    glVertex3f(x + width, y - height, z - length);
    glVertex3f(x, y - height, z - length);
    glEnd();
}

void drawHexagonalPrism(GLfloat x[], GLfloat z[], GLfloat y, GLfloat h){
    glBegin(GL_QUADS);
    for (int i1 = 0; i1 < 6; ++i1)
    {
        glColor4f(
            i1 < 2 || i1 > 4 ? 1.0f : 0.0f,
            i1 > 0 && i1 < 5 ? 1.0f : 0.0f,
            i1 > 2 ? 1.0f : 0.0f,
            1.0f
        );

        int i2 = (i1 + 1) % 6;
        glVertex3f(x[i1], y, z[i1]);
        glVertex3f(x[i2], y, z[i2]);
        glVertex3f(x[i2], y + h, z[i2]);
        glVertex3f(x[i1], y + h, z[i1]);
    }
    glEnd();

    glColor4f(1, 1, 1, 1);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i)
        glVertex3f(x[i], y, z[i]);
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i)
        glVertex3f(x[i], y + h, z[i]);
    glEnd();

}


void drawRightBuilding() {
    glPushMatrix();

    //columns left to right
    drawCube(15, 5, 6, 4, 5, 0.5);

    drawCube(15, 5, 4.5, 4, 5, 0.5);
    drawCube(15, 5.5, 4, 4, 5.5, 0.5);

    drawCube(15, 5.5, 2.5, 4, 5.5, 0.5);
    drawCube(15, 6, 2, 4, 6, 0.5);

    drawCube(15, 6, 0, 4, 6, 1.0); //center

    drawCube(15, 6, -2, 4, 6, 0.5);
    drawCube(15, 5.5, -2.5, 4, 5.5, 0.5);

    drawCube(15, 5, -4.5, 4, 5, 0.5);
    drawCube(15, 5.5, -4, 4, 5.5, 0.5);

    drawCube(15, 5, -6, 4, 5, 0.5);

    //rows top to bottom
    drawCube(15, 6, -2, 4, 0.5, -4); //top two rows
    drawCube(15, 5.5, -4, 4, 0.5, -8);

    for (GLfloat y = 5; y >= 0.5; y -= 1.5) {
        drawCube(15, y, 6, 4, 0.5, 12);

    }
    //drawCube(5, 3.5, -6, 4, 0.5, 12);
    //drawCube(5, 2, -6, 4, 0.5, 12);
    //drawCube(5, 0.5, -6, 4, 0.5, 12);

    //window rows top to bottom
    drawCube(15.2, 4.5, 6, 3.6, 1, 12);
    for (GLfloat y = 3; y >= 1.5; y -= 1.5) {
        drawCube(15, y, 6, 3.8, 1, 12);

    }
    //drawCube(5.8, 3, -6, 3, 1, 12);
    //drawCube(5.8, 1.5, -6, 3, 1, 12);

    glPopMatrix();
}

void drawRightSideCornerBuilding() {
    glPushMatrix();

    //4 corner beams (columns)
    drawCube(15, 7, 10, 1, 7, 1);
    drawCube(18, 7, 10, 1, 7, 1);
    drawCube(18, 7, 7, 1, 7, 1);
    drawCube(15, 7, 7, 1, 7, 1);

    drawCube(15, 7, 9.5, 3.5, 7, 3.5); //core


    //2 small beams in front and left side (columns)
    drawCube(16, 7, 9.8, 0.5, 7, 0.8);//front
    drawCube(17.5, 7, 9.8, 0.5, 7, 0.8);
    drawCube(18, 7, 9, 0.8, 7, 0.5);//right
    drawCube(18, 7, 7.5, 0.8, 7, 0.5);

    //wall rows
    //right side (top to bottom)
    drawCube(18, 7, 8.5, 0.8, 0.5, 1);
    drawCube(18, 5.5, 8.5, 0.8, 1, 1);
    drawCube(18, 3.5, 8.5, 0.8, 1, 1);
    drawCube(18, 0.5, 8.5, 0.8, 0.5, 1);

    //front side (top to bottom)
    drawCube(16.5, 7, 9.8, 1, 0.5, 0.8);
    drawCube(16.5, 5.5, 9.8, 1, 1, 0.8);
    drawCube(16.5, 3.5, 9.8, 1, 1, 0.8);
    drawCube(16.5, 0.5, 9.8, 1, 0.5, 0.8);

    //small balcony parts in middle
    drawCube(15, 5.1, 10, 4, 0.2, 4);
    drawCube(15, 3.1, 10, 4, 0.2, 4);

    //draw the hexagon part in the top
    GLfloat x[] = { 16.134f, 16.134f, 17.0f, 17.866f, 17.866f, 17.0f };
    GLfloat z[] = { 7.5f, 8.5f, 9.0f, 8.5f, 7.5f, 7.0f };
    GLfloat y = 7.0f;
    GLfloat h = 1.0f;

    //draw top balcony
    drawCube(14.9, 7.1, 10.2, 4.2, 0.1, 4.3);
    drawCube(15.2, 7.4, 9.8, 3.6, 0.4, 0.2);
    drawCube(15.2, 7.4, 9.8, 0.2, 0.4, 3.6);
    drawCube(15.2, 7.4, 6.2, 3.6, 0.4, 0.2);
    drawCube(18.6, 7.4, 9.8, 0.2, 0.4, 3.6);

    //draw hexagon building on top
    drawHexagonalPrism(x, z, y, h);

    //cone roof part on the hexagon
    glPushMatrix();
    glTranslatef(17, 8, 8);
    glRotatef(90, -1.0, 0, 0);
    glutSolidCone(0.5, 1.3, 6, 3);
    glutSolidCone(1.3, 1, 6, 3);
    glPopMatrix();

    glPopMatrix();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Depth Buffer should be cleared everytime a redraw happens
    glPushMatrix();

    //camera position
    gluLookAt(0.0 + camX, 2.0 + camY, 5.0 + camZ, 0, 0, 0, 0, 1.0, 0);

    // move the object frame using keyboard keys
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);


    glColor3f(1.0, 1.0, 1.0);

    drawRightBuilding();
    drawRightSideCornerBuilding();
    drawCube(5, 6, 10, 10, 6, 4);//front right

    
    drawCube(5, 5, 6, 4, 5, 12.5);//inner left middle
    drawCube(9, 3, 6, 6, 3, 12.5);//inner right middle
    drawCube(-7, 5, 10, 12, 5, 4);//front center
    drawCube(-10, 5, 10, 4, 5, 16.5);//right
    drawCube(-10, 5, -6.5, 19, 5, 4);//back


    drawGrid(); //draw the grids
    drawAxes(); //draw the xyz axes

    glPopMatrix();                 
    glutSwapBuffers();
}

void changeSize(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 500);
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



