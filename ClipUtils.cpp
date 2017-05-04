//
// Created by mike on 5/2/17.
//
#include "ClipUtils.h"


//checks if point lies inside
bool inside(Point vPoint, Point bPoint, BoundaryType bType) {
    switch (bType) {
        case MAX_X:
            return (vPoint.x <= bPoint.x);
        case MAX_Y:
            return (vPoint.y <= bPoint.y);
        case MIN_X:
            return (vPoint.x >= bPoint.x);
        case MIN_Y:
            return (vPoint.y >= bPoint.y);
    }
    return false;
}

//calculates intersection
Point intersection(Point start, Point end, Point bPoint, BoundaryType bType) {
    Point iPoint;
    float slope = (end.y - start.y) / (end.x - start.x);

    switch (bType) {
        case MAX_X:
            iPoint.x = bPoint.x;
            iPoint.y = slope * (bPoint.x - start.x) + start.y;
            break;
        case MAX_Y:
            iPoint.x = start.x + (bPoint.y - start.y) / slope;
            iPoint.y = bPoint.y;
            break;
        case MIN_X:
            iPoint.x = bPoint.x;
            iPoint.y = slope * (bPoint.x - start.x) + start.y;
            break;
        case MIN_Y:
            iPoint.x = start.x + (bPoint.y - start.y) / slope;
            iPoint.y = bPoint.y;
            break;
    }
    return iPoint;
}

//the main Shcp algorithm
int SHPC(int in, const Point inputPoints[], Point outV[], Point edgePoint, BoundaryType bType) {
    Point endPoint;
    Point startPoint = inputPoints[in - 1];

    // Final out count
    int outPointCount = 0;

    // Iterate through through vertices
    for (int i = 0; i < in; i++) {
        endPoint = inputPoints[i];
        if (inside(endPoint, edgePoint, bType)) {
            if (inside(startPoint, edgePoint, bType)) {
                // Case 1, no intersection
                outV[outPointCount] = endPoint;
                outPointCount++;
            } else {
                // Case 4, outside to inside intersection
                outV[outPointCount] = intersection(startPoint, endPoint, edgePoint, bType);
                outPointCount++;
                outV[outPointCount] = endPoint;
                outPointCount++;
            }
        } else {
            if (inside(startPoint, edgePoint, bType)) {
                // Case 2, inside to outside intersection
                outV[outPointCount] = intersection(startPoint, endPoint, edgePoint, bType);
                outPointCount++;
            }
            // Case 3, Edge is completely outside of clipping window
        }
        startPoint = endPoint;
    }
    return (outPointCount);
}

int clipPolygon(int in, const Point inV[], Point outV[],
                Point ll, Point ur) {
    Point out1[50];
    Point out2[50];
    Point out3[50];

    int outVCount = 0;

    outVCount = SHPC(in, inV, out1, ur, MAX_X);
    outVCount = SHPC(outVCount, out1, out2, ur, MAX_Y);
    outVCount = SHPC(outVCount, out2, out3, ll, MIN_Y);
    outVCount = SHPC(outVCount, out3, outV, ll, MIN_X);

    return (outVCount);
}

float getArea(Point v1, Point v2, Point v3) {
    return (float) (fabs((v1.x - v3.x) * (v2.y - v1.y) - (v1.x - v2.x) * (v3.y - v1.y)) / 2);
}

