#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xutil.h>
#include "Sphere.h"
#include "Rasterizer.h"

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

char WINDOW_NAME[] = "Graphics Window";
char ICON_NAME[] = "Icon";
Display *display;
int screen;
Window main_window;
GC gc;
unsigned long foreground, background;

int x_es[3] = {0, 0, 0};
int y_es[3] = {0, 0, 0};
int counter = 0;

Point LL = Point();
Point TR = Point();

Rasterizer rast;


void connectX() {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot connect to X\n");
        exit(1);
    }
    screen = DefaultScreen(display);
    foreground = BlackPixel(display, screen);
    background = WhitePixel(display, screen);
}

Window openWindow(int x, int y, int width, int height,
                  int border_width, int argc, char **argv) {
    Window new_window;
    XSizeHints size_hints;
    new_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                                     x, y, (unsigned int) width, (unsigned int) height, (unsigned int) border_width,
                                     foreground,
                                     background);
    size_hints.x = x;
    size_hints.y = y;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.flags = PPosition | PSize;
    XSetStandardProperties(display, new_window, WINDOW_NAME, ICON_NAME,
                           None, argv, argc, &size_hints);
    XSelectInput(display, new_window, (ButtonPressMask | KeyPressMask |
                                       ExposureMask | PointerMotionMask | StructureNotifyMask));
    return (new_window);
}

GC getGC() {
    GC gc;
    XGCValues gcValues;
    gc = XCreateGC(display, main_window, (unsigned long) 0, &gcValues);
    XSetBackground(display, gc, background);
    XSetForeground(display, gc, foreground);
    return (gc);
}

void disconnectX() {
    XCloseDisplay(display);
    exit(0);
}


void drawInstructions() {
    XClearWindow(display, main_window);
    XDrawRectangle(display, main_window, gc, LL.x, LL.y, TR.x, TR.y);
    XColor col;
    col.pixel = ((100 & 0xff) | ((50 & 0xff) << 8) | ((30 & 0xff) << 16));
    /*For true color do n o t do XallocColor or such */
    XSetForeground(display, gc, col.pixel);
    // XFlush(display);
    XDrawString(display, main_window, gc, 20, 330, "Press S to draw Sphere", strlen("Press S to draw Sphere"));
    XDrawString(display, main_window, gc, 20, 340, "Press Mouse 3 times to draw triangle",
                strlen("Press Mouse 3 times to draw triangle"));
    XDrawString(display, main_window, gc, 20, 350, "Press Q to exit", strlen("Press Q to exit"));

}

void doButtonPressEvent(XButtonEvent *pEvent) {


    x_es[counter] = pEvent->x;
    y_es[counter] = pEvent->y;
    printf("%i , ", pEvent->x);
    printf("%i , ", pEvent->y);

    if (counter == 2) {
        XFlush(display);
        drawInstructions();

        Color color1(1.0f, 0.0f, 0.0f);
        Color color2(0.0f, 1.0f, 0.0f);
        Color color3(0.0f, 0.0f, 1.0f);

        rast.DrawTriangle(color1, x_es[0], y_es[0], color2, x_es[1], y_es[1], color3, x_es[2], y_es[2]);

        counter = 0;
        return;
    }


    counter += 1;
    /* disconnectX();*/
}

void doExposeEvent(XExposeEvent *pEvent) {
    printf("Expose Event occurred\n");
    drawInstructions();
    // draw();
}

void doKeyPressEvent(XKeyEvent *pEvent) {
    int key_buffer_size = 10;
    char key_buffer[9];
    XComposeStatus compose_status;
    KeySym key_sym;
    XLookupString(pEvent, key_buffer, key_buffer_size,
                  &key_sym, &compose_status);
    if (key_buffer[0] == 'q') disconnectX();

    if (key_buffer[0] == 's') {
        XClearWindow(display, main_window);
        drawInstructions();
        Sphere sphere(200);
        int size = sphere.getFinalSize();
        int *points = sphere.getFinalArr();


        //Using Triangle strips
        for (int i = 0; i < size - 5; i += 2) {
            float factor = (float) i / (float) size;
            Color color(1 - factor, 1 - factor, 1 - factor);
            rast.DrawTriangle(points[i], points[i + 1], points[i + 2], points[i + 3], points[i + 4], points[i + 5],
                              color);
        }


    } else printf("You pressed %c\n", key_buffer[0]);
}

void doReconfigure(XConfigureEvent *event) {
    printf("Mouse clicked at %d,%d\n", event->width, event->height);
}

int main(int argc, char **argv) {
    XEvent event;
    connectX();
    main_window = openWindow(10, 20, 500, 500, 5, argc, argv);
    gc = getGC();
    XMapWindow(display, main_window);
    XFlush(display);
    LL.x = 0;
    LL.y = 0;
    TR.x = 300;
    TR.y = 300;
    rast.setWindowParams(display, main_window, gc, LL, TR);
    while (True) {
        XNextEvent(display, &event);
        switch (event.type) {
            case Expose:
                doExposeEvent(&event.xexpose);
                break;
            case ConfigureNotify:
                printf("ConfigureNotify\n");
                doReconfigure(&event.xconfigure);
                break;
            case ButtonPress:
                printf("Button Pressed\n");
                doButtonPressEvent(&event.xbutton);
                break;
            case KeyPress:
                printf("KeyPressed");
                doKeyPressEvent(&event.xkey);
                break;
        }

    }
}
