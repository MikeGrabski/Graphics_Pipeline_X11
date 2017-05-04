//
// Created by mike on 5/2/17.
//

#include "RasterUtils.h"


Color::Color(float r, float g, float b, float a) {
    R = r;
    G = g;
    B = b;
    A = a;
}

Color
Color::operator+(const Color &c) const {
    return Color(R + c.R, G + c.G, B + c.B, A + c.A);
}

Color
Color::operator-(const Color &c) const {
    return Color(R - c.R, G - c.G, B - c.B, A - c.A);
}

Color
Color::operator*(float f) const {
    return Color(R * f, G * f, B * f, A * f);
}


Edge::Edge(const Color &color1, int x1, int y1,
           const Color &color2, int x2, int y2) {
    if (y1 < y2) {
        Color1 = color1;
        Color2 = color2;
        X1 = x1;
        Y1 = y1;
        X2 = x2;
        Y2 = y2;
    } else {
        Color1 = color2;
        Color2 = color1;
        X1 = x2;
        Y1 = y2;
        X2 = x1;
        Y2 = y1;
    }
}

Span::Span(const Color &color1, int x1, const Color &color2, int x2) {
    if (x1 < x2) {
        Color1 = color1;
        X1 = x1;
        Color2 = color2;
        X2 = x2;
    } else {
        Color1 = color2;
        X1 = x2;
        Color2 = color1;
        X2 = x1;
    }
}