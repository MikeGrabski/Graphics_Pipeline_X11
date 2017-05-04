//
// Created by mike on 5/4/17.
//

#include <cstdio>
#include <X11/Xlib.h>
#include "Rasterizer.h"


void Rasterizer::setColor(int red, int green, int blue) {
    XColor col;
    col.pixel = (unsigned long) ((blue & 0xff) | ((green & 0xff) << 8) | ((red & 0xff) << 16));
    /*For true color do n o t do XallocColor or such */
    XSetForeground(display, gc, col.pixel);
}

void
Rasterizer::SetPixel(int x, int y, const Color &color) {
    /*For true color do n o t do XallocColor or such */
    setColor(color.R * 255, color.G * 255, color.B * 255);
    XDrawPoint(display, main_window, gc, x, y);
}


void
Rasterizer::DrawSpan(const Span &span, int y) {
    int xdiff = span.X2 - span.X1;
    if (xdiff == 0)
        return;

    Color colordiff = span.Color2 - span.Color1;

    float factor = 0.0f;
    float factorStep = 1.0f / (float) xdiff;

    // draw each pixel in the span
    for (int x = span.X1; x < span.X2; x++) {
        SetPixel(x, y, span.Color1 + (colordiff * factor));
        factor += factorStep;
    }
}

void
Rasterizer::DrawSpansBetweenEdges(const Edge &e1, const Edge &e2) {
    float e1ydiff = (float) (e1.Y2 - e1.Y1);
    if (e1ydiff == 0.0f)
        return;

    // calculate difference between the y coordinates
    // of the second edge and return if 0
    float e2ydiff = (float) (e2.Y2 - e2.Y1);
    if (e2ydiff == 0.0f)
        return;

    // calculate differences between the x coordinates
    // and colors of the points of the edges
    float e1xdiff = (float) (e1.X2 - e1.X1);
    float e2xdiff = (float) (e2.X2 - e2.X1);
    Color e1colordiff = (e1.Color2 - e1.Color1);
    Color e2colordiff = (e2.Color2 - e2.Color1);

    // calculate factors to use for interpolation
    // with the edges and the step values to increase
    // them by after drawing each span
    float factor1 = (float) (e2.Y1 - e1.Y1) / e1ydiff;
    float factorStep1 = 1.0f / e1ydiff;
    float factor2 = 0.0f;
    float factorStep2 = 1.0f / e2ydiff;

    // loop through the lines between the edges and draw spans
    for (int y = e2.Y1; y < e2.Y2; y++) {
        // create and draw span
        Span span(e1.Color1 + (e1colordiff * factor1),
                  e1.X1 + (int) (e1xdiff * factor1),
                  e2.Color1 + (e2colordiff * factor2),
                  e2.X1 + (int) (e2xdiff * factor2));
        DrawSpan(span, y);

        // increase factors
        factor1 += factorStep1;
        factor2 += factorStep2;
    }
}

Color getColor(Point point, Point v1, Point v2, Point v3, float Area) {
    float blue = getArea(point, v1, v2) / Area;
    float red = getArea(point, v2, v3) / Area;
    float green = getArea(point, v1, v3) / Area;
    Color c(red, green, blue);
    return c;
}

//Multicolor Triangle
void
Rasterizer::DrawTriangle(Color &color1, float x1, float y1,
                         Color &color2, float x2, float y2,
                         Color &color3, float x3, float y3) {
    // create edges for the triangle
    Point in[3];
    //8 because max amount of times a triangle can cross a rectangle is 8
    Point out[8];

    in[0].x = x1;
    in[0].y = y1;

    in[1].x = x2;
    in[1].y = y2;

    in[2].x = x3;
    in[2].y = y3;

    int result = clipPolygon(3, in, out, LL, TR);

    for (int i = 0; i < result; ++i) {
        printf("x: ");
        printf("%f", out[i].x);
        printf(" y: ");
        printf("%f", out[i].y);
        printf("\n");
    }
    float triangleArea = getArea(in[0], in[1], in[2]);
    for (int i = 2; i < result; ++i) {
        color2 = getColor(out[i - 1], in[0], in[1], in[2], triangleArea);
        color3 = getColor(out[i], in[0], in[1], in[2], triangleArea);
        Edge edges[3] = {
                Edge(color1, (int) out[0].x, (int) out[0].y, color2, (int) out[i - 1].x, (int) out[i - 1].y),
                Edge(color2, (int) out[i - 1].x, (int) out[i - 1].y, color3, (int) out[i].x, (int) out[i].y),
                Edge(color3, (int) out[i].x, (int) out[i].y, color1, (int) out[0].x, (int) out[0].y)
        };

        int maxLength = 0;
        int longEdge = 0;

        //     find edge with the greatest length in the y axis
        for (int i = 0; i < 3; i++) {
            int length = edges[i].Y2 - edges[i].Y1;
            if (length > maxLength) {
                maxLength = length;
                longEdge = i;
            }
        }

        int shortEdge1 = (longEdge + 1) % 3;
        int shortEdge2 = (longEdge + 2) % 3;

        // draw spans between edges; the long edge can be drawn
        // with the shorter edges to draw the full triangle
        DrawSpansBetweenEdges(edges[longEdge], edges[shortEdge1]);
        DrawSpansBetweenEdges(edges[longEdge], edges[shortEdge2]);
    }
}


//Single color triangle
void
Rasterizer::DrawTriangle(float x1, float y1,
                         float x2, float y2,
                         float x3, float y3, Color color) {
    // create edges for the triangle
    Point in[3];
    Point out[10];

    in[0].x = x1;
    in[0].y = y1;

    in[1].x = x2;
    in[1].y = y2;

    in[2].x = x3;
    in[2].y = y3;

    int result = clipPolygon(3, in, out, LL, TR);
    int i = 1;
    for (int i = 0; i < result; ++i) {
        printf("x: ");
        printf("%f", out[i].x);
        printf(" y: ");
        printf("%f", out[i].y);
        printf("\n");
    }
    Edge edges[3] = {
            Edge(color, (int) x1, (int) y1, color, (int) x2, (int) y2),
            Edge(color, (int) x2, (int) y2, color, (int) x3, (int) y3),
            Edge(color, (int) x3, (int) y3, color, (int) x1, (int) y1)
    };

    int maxLength = 0;
    int longEdge = 0;

    //     find edge with the greatest length in the y axis
    for (int i = 0; i < 3; i++) {
        int length = edges[i].Y2 - edges[i].Y1;
        if (length > maxLength) {
            maxLength = length;
            longEdge = i;
        }
    }

    int shortEdge1 = (longEdge + 1) % 3;
    int shortEdge2 = (longEdge + 2) % 3;

    // draw spans between edges; the long edge can be drawn
    // with the shorter edges to draw the full triangle
    DrawSpansBetweenEdges(edges[longEdge], edges[shortEdge1]);
    DrawSpansBetweenEdges(edges[longEdge], edges[shortEdge2]);

}

Rasterizer::Rasterizer(Display *dis, Window main_window, GC gc, Point LL, Point TR) {
    this->display = dis;
    this->main_window = main_window;
    this->gc = gc;
    this->LL = LL;
    this->TR = TR;
}

Rasterizer::Rasterizer() {

}

void Rasterizer::setWindowParams(Display *display, Window main_window, GC gc, Point LL, Point TR) {
    this->display = display;
    this->main_window = main_window;
    this->gc = gc;
    this->LL = LL;
    this->TR = TR;
}
