//
// Created by root on 4/30/17.
//

#ifndef UNTITLED_RASETER_UTILITIES_H
#define UNTITLED_RASETER_UTILITIES_H

#endif //UNTITLED_RASETER_UTILITIES_H


class Color {
public:
    float R, G, B, A;

    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

    Color operator+(const Color &c) const;

    Color operator-(const Color &c) const;

    Color operator*(float f) const;
};



class Edge {
public:
    Color Color1, Color2;
    int X1, Y1, X2, Y2;

    Edge(const Color &color1, int x1, int y1, const Color &color2, int x2, int y2);
};

class Span {
public:
    Color Color1, Color2;
    int X1, X2;

    Span(const Color &color1, int x1, const Color &color2, int x2);
};

