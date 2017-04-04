#ifndef ESFERA_H
#define ESFERA_H

/*
Una clase que construye los datos: vertices, normales y 
coordenadas de textura de una Esfera
*/

class Esfera {
public:
    Esfera(float, int, int);

    ~Esfera();

    int N, M;
    int n_triangulos;
    int n_vertices;
    float *vertices;
    float *normal;
    float *textCoord;
    int *index;

protected:
    double *theta, *phi;
    double R;

    int getindex(int, int);
};

#endif