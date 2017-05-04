//
// Created by mike on 5/4/17.
//

#ifndef UNTITLED_RASTERIZER_H
#define UNTITLED_RASTERIZER_H


#include "RasterUtils.h"
#include "ClipUtils.h"


class Rasterizer {
private:
    Display *display; Window main_window; GC gc; Point LL; Point TR;

protected:
    void DrawSpan(const Span &span, int y);

    void DrawSpansBetweenEdges(const Edge &e1, const Edge &e2);

public:
    void SetPixel(int x, int y, const Color &color = Color());

    void DrawTriangle(Color &color1, float x1, float y1, Color &color2, float x2, float y2, Color &color3, float x3,
                      float y3);

    void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, Color color);

    void setColor(int red, int green, int blue);

    Rasterizer(Display *dis, Window main_window, GC gc, Point LL, Point TR);

    Rasterizer();

    void setWindowParams(Display *display, Window main_window, GC gc, Point LL, Point TR);
};


#endif //UNTITLED_RASTERIZER_H
