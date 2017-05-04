//
// Created by root on 4/30/17.
//

#include <cmath>

#ifndef UNTITLED_CLIPUTILS_H
#define UNTITLED_CLIPUTILS_H

#endif //UNTITLED_CLIPUTILS_H
typedef struct point {
    float x;
    float y;
} Point;

enum BoundaryType {
    MAX_X, MAX_Y, MIN_X, MIN_Y
};

//checks if point lies inside
bool inside(Point vPoint, Point bPoint, BoundaryType bType);

//calculates intersection
Point intersection(Point start, Point end, Point bPoint, BoundaryType bType);

//the main Shcp algorithm
int SHPC(int in, const Point inputPoints[], Point outV[], Point edgePoint, BoundaryType bType);
int clipPolygon(int in, const Point inV[], Point outV[],
                Point ll, Point ur);
float getArea(Point v1, Point v2, Point v3);

