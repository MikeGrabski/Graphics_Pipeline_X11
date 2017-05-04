//
// Created by mike on 5/2/17.
//

#ifndef UNTITLED_SPHERE_H
#define UNTITLED_SPHERE_H


#include <cmath>
#include <list>

class Vertex {
public:
    Vertex(float X, float Y, float Z) {
        x = X;
        y = Y;
        z = Z;
    }

    float z;
    float x;
    float y;

    float* getPerspective(){
        float *v = new float[4];
        v[0] = this->x;
        v[1] = this->y;
        v[2] = this->z;
        v[3] = 1;
        return v;
    }
};

class Sphere {

public:

    Sphere(int n);


    int getFinalSize(){
        return finalSize;
    }

    int* getFinalArr(){
        return finalPoints;
    }

private:
    int n;
    std::list<Vertex> vertices;

private:
    void output_vertex(int lat, int lon);

    void gen_triangles();

    void project();

    void genOrthoProjection(float l, float r, float b, float t, float n, float f);

    float proj[4][4];

    float* vertexProj(float *v);

    void print();

    int finalSize;

    int* finalPoints;
};


#endif //UNTITLED_SPHERE_H
