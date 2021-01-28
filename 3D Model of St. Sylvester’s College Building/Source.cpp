#include <iostream>
#include <GL/glut.h>

GLfloat x = 6.0f;
GLfloat y = 6.0f;
GLfloat z = 6.0f;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);                // Enable the Depth testig in openGL
    glEnable(GL_CULL_FACE);                 // Enable Hidden Surface Removal
    glPolygonMode(GL_FRONT, GL_FILL);       // Set the Front of the faces of the cube to be unfilled (Skeleton Model)
    glPolygonMode(GL_BACK, GL_FILL);        // Set the Back of the faces of the cube to be filled with the color

}


void aWall() {

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Depth Buffer should be cleared everytime a redraw happens

    glPushMatrix();                                //
    glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);          //      Rotation of the Coordinate space
    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);           //


    // WALL FRONT
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-5, 2, -6);
    glVertex3f(-5, -2, -6);
    glVertex3f(5, -2, -6);
    glVertex3f(5, 2, -6);
    glEnd();

    // WALL BACK
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.3f, 0.5f);
    glVertex3f(-5, 2, -4);
    glVertex3f(-5, -2, -4);
    glVertex3f(5, -2, -4);
    glVertex3f(5, 2, -4);
    glEnd();



    // WALL LEFT
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-5, 2, -6);
    glVertex3f(-5, -2, -6);
    glVertex3f(-5, -2, -4);
    glVertex3f(-5, 2, -4);
    glEnd();


    // WALL RIGHT
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(5, 2, -6);
    glVertex3f(5, -2, -6);
    glVertex3f(5, -2, -4);
    glVertex3f(5, 2, -4);
    glEnd();


    // WALL TOP
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-5, 2, -6);
    glVertex3f(5, 2, -6);
    glVertex3f(5, 2, -4);
    glVertex3f(-5, 2, -4);
    glEnd();

    //WALL BOTTOM
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-5, -2, -6);
    glVertex3f(5, -2, -6);
    glVertex3f(5, -2, -4);
    glVertex3f(-5, -2, -4);
    glEnd();

    glPopMatrix();                 
    glutSwapBuffers();
}

void changeSize(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);

    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h)
        glOrtho(-10.0, 10.0, -10 / aspect_ratio, 10 / aspect_ratio, 10.0, -10.0);
    else
        glOrtho(-10.0 * aspect_ratio, 10.0 * aspect_ratio, -10.0, 10.0, 10.0, -10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(320, 320);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("St. Sylvester's College Building");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    init();
    glutMainLoop();


    return 0;
}



