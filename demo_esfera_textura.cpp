#include <stdio.h>
#include <cmath>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Esfera.h"
#include "getbmp.h"


#define WINDOW_ANCHO 500
#define WINDOW_ALTO 500
#define IMAGEN "/home/gabriel/ClionProjects/PR10T2/pluton.bmp"

// Espacio para las variables globales de la ventana
float gl_fovy = 60.0f;
float gl_fovy_min = 1.0f, gl_fovy_max = 120.0f, gl_fovy_paso = 1.0f;
float gl_cerca = 1.0f, gl_lejos = 100.0f;
float gl_center = 7.0f; // centro donde pondremos los objetos

// Espacio para otras variables globales
double theta = M_PI_2, phi = 0.0;
double radio = 10.0;
Esfera *esfera;
BitMapFile *imagen;

// Espacio para la declaraci�n de funciones
void InitGlew();

void InitGL();

void Display();

void ReshapeSize(int ancho, int alto);

void PosicionRaton(int x, int y);

void RuedaRaton(int rueda, int sentido, int x, int y);

void setTexture(char *filename);

//main
int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_ANCHO, WINDOW_ALTO);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("OpenGL Practica 12_2");
    InitGlew(); // despues de crear la primera ventana
    InitGL();

    glutDisplayFunc(Display); // registra la funcion de rendering
    glutReshapeFunc(ReshapeSize);
    glutMotionFunc(PosicionRaton);
    glutMouseWheelFunc(RuedaRaton);

    esfera = new Esfera(1.0f, 500, 500);

    glutMainLoop(); // bucle principal

    delete esfera;

    return 0;
}

void InitGlew() {

    // para poder utilizar trasnparentemente todas las extensiones de OpenGL

    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_init));
    } else
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void setViewport() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, ancho, alto); // utiliza la totalidad de la ventana
}

void setProjection() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(gl_fovy, (float) ancho / (float) alto, gl_cerca, gl_lejos);
}

void setModelView() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void setLight0() {

    float ambient[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    float position[4] = {1.0f, -1.0f, 10.0f, 1.0f};
    float direction[4] = {-1.0f, -1.0f, -1.0f, 1.0f};
    float cut_off = 180.0f;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cut_off);

    glEnable(GL_LIGHT0);
}

void setTexture(char *filename) {

    // lectura de la imagen en un buffer
    imagen = getbmp(filename);
    int ancho = imagen->sizeX;
    int alto = imagen->sizeY;

    // habilitar la textura
    glEnable(GL_TEXTURE_2D);

    // aunque en nuestro caso las coordenadas (s,t) estan ajustadas a [0,1]
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagen->data);

}


void InitGL() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // para cambiar el criterio de lo que se ve.
    // LESS es el de defecto: se ve lo que tiene menor profundidad
    glDepthFunc(GL_LESS);
    // TO DO

    setViewport();
    setProjection();

    glEnable(GL_LIGHTING);
    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);

    setLight0();
    //setTexture("earth_high.bmp");
    setTexture((char *) IMAGEN);

}

// cambio de dimensiones de la ventana, manteniendo la dimension de los objetos
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void ReshapeSize(int ancho, int alto) {
#pragma clang diagnostic pop

    setViewport();
    setProjection();
    glutPostRedisplay();
}

void PosicionRaton(int x, int y) {

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    theta = M_PI * (double) y / (double) alto;
    phi = -M_PI + 2 * M_PI * (double) x / (double) ancho;
    glutPostRedisplay();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

void RuedaRaton(int rueda, int sentido, int x, int y) {
#pragma clang diagnostic pop

    gl_fovy -= gl_fovy_paso * (float) sentido;
    if (gl_fovy < gl_fovy_min) gl_fovy = gl_fovy_min;
    if (gl_fovy > gl_fovy_max) gl_fovy = gl_fovy_max;
    setProjection();
    glutPostRedisplay();
}

void setMaterial() {

    const GLfloat ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    const GLfloat diffuse[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    const GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat shininess = 100.0f;


    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// funci�n de gestion de ventana
void Display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra todo lo existente en el framebuffer
    setModelView();

    // poicionamos el centro de coordenadas
    glTranslatef(0.0f, 0.0f, -gl_center);

    // el eje cenital es el Y
    float x = (float) (radio * sin(theta) * sin(phi));
    float y = (float) (radio * cos(theta));
    float z = (float) (radio * sin(theta) * cos(phi));
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // mira al (0,0,0)

    setMaterial();


    // TO DO
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // eje z arriba

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, esfera->vertices);
    glNormalPointer(GL_FLOAT, 0, esfera->normal);
    glTexCoordPointer(2, GL_FLOAT, 0, esfera->textCoord);

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3 * esfera->n_triangulos, GL_UNSIGNED_INT, esfera->index);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glFlush(); // actualiza el framebuffer
    glutSwapBuffers(); // en caso de animacion
}

