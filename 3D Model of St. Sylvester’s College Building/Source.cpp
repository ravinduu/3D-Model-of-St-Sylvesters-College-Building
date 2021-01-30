#include <iostream>
#include <GL/glut.h>

GLfloat windowW = 20;
GLfloat windowH = 20;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);                // Enable the Depth testig in openGL
    glEnable(GL_CULL_FACE);                 // Enable Hidden Surface Removal
    glPolygonMode(GL_FRONT, GL_FILL);       // Set the Front of the faces of the cube to be unfilled (Skeleton Model)
    glPolygonMode(GL_BACK, GL_FILL);        // Set the Back of the faces of the cube to be filled with the color

}


void displayWall(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat length) {
    
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
    glVertex3f(x, y, z + length);
    glVertex3f(x, y - height, z + length);
    glVertex3f(x + width, y - height, z + length);
    glVertex3f(x + width, y, z + length);
    glEnd();



    // WALL LEFT
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(x , y, z);
    glVertex3f(x, y - height, z);
    glVertex3f(x, y - height, z + length);
    glVertex3f(x, y, z + length);
    glEnd();


    // WALL RIGHT
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(x + width, y, z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width, y - height, z + length);
    glVertex3f(x + width, y, z + length);
    glEnd();


    // WALL TOP
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(x, y, z);
    glVertex3f(x + width, y - height, z);
    glVertex3f(x + width, y, z + length);
    glVertex3f(x, y, z + length);
    glEnd();


    //WALL BOTTOM
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(x, y - height, z);
    glVertex3f(x + width, y - height , z);
    glVertex3f(x + width, y - height, z + length);
    glVertex3f(x, y - height, z + length);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Depth Buffer should be cleared everytime a redraw happens
    glPushMatrix();                                //
    
    glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);          //      Rotation of the Coordinate space
    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);           //

    
    displayWall(-5, 2, -6, 10, 4, 2);
    displayWall(3, 2, -6, 2, 4, 20);
    displayWall(-5, 2, -6, 2, 4, 20);
    displayWall(-5, 2, 14, 10, 4, 2);




    glPopMatrix();                 
    glutSwapBuffers();
}

void changeSize(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);

    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();




    if (w <= h)
        glOrtho(-windowW, windowW, -windowH / aspect_ratio, windowH / aspect_ratio, 20.0, -20.0);
    else
        glOrtho(-windowW * aspect_ratio, windowW * aspect_ratio, -windowH, windowH, 20.0, -20.0);

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
    init();
    glutMainLoop();


    return 0;
}



