//
// Created by mike on 5/2/17.
//


#include <cstdio>
#include "Sphere.h"


Sphere::Sphere(int n) {
    this->n = n;
    gen_triangles();
    genOrthoProjection(-1.5, 1.5, -1.5, 1.5, 1, 20);
    project();
}

void Sphere::output_vertex(int lat, int lon) {
    float v[3];
    double angleLat, angleLon;

    angleLat = 2. * M_PI * lat / n;
    angleLon = 2. * M_PI * lon / n;
    v[0] = cos(angleLon) * sin(angleLat);
    v[1] = sin(angleLon) * sin(angleLat);
    v[2] = cos(angleLat);
    Vertex vertex(v[0], v[1], v[2]);
    vertices.push_back(vertex);

}

void Sphere::gen_triangles() {
    int lat, lon;
    for (lat = 0; lat < n / 2; lat++) {
        for (lon = 0; lon <= n; lon++) {
            output_vertex(lat, lon);
            output_vertex(lat + 1, lon);
        }
    }
}


void Sphere::project() {
    std::list<Vertex>::iterator iterator;
    int *final_arr = new int[vertices.size() * 2];
    int i = 0;
    for (iterator = vertices.begin(); iterator != vertices.end(); ++iterator) {
        float *vertex = vertexProj((*iterator).getPerspective());
        if (vertex[2] > 0) {}
        vertex[0] = vertex[0] * vertex[2];
        vertex[1] = vertex[1] * vertex[2];
        final_arr[i] = (vertex[0] + 1) * 300/2;
        final_arr[i + 1] = (vertex[1] + 1) * 300/2;
        i += 2;
    }
    finalPoints = final_arr;
    finalSize = i / 2;

}

void Sphere::genOrthoProjection(float l, float r, float b, float t, float n, float f) {
    float projectionMat[4][4] = {
            {2.f / (r - l), 0,             0,              -(r + l) / (r - l)},
            {0,             2.f / (t - b), 0,              -(t + b) / (t - b)},
            {0,             0,             -2.f / (f - n), -(f + n) / (f - n)},
            {0,             0,             0,              1}
    };
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            proj[i][j] = projectionMat[i][j];

}


float *Sphere::vertexProj(float *v) {
    float *c = new float[4];
    for (int i = 0; i < 4; i++) {
        c[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c[i] += (proj[i][j] * v[j]);
        }
    }
    return c;
}
