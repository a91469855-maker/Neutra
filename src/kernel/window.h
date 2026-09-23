#ifndef WINDOW_H
#define WINDOW_H

typedef struct {
    int x;
    int y;
    int width;
    int height;
    const char* title;
} Window;

void window_init(void);
void window_draw_background(void);
void window_draw(Window* win);
void window_set_position(Window* win, int x, int y);
void window_move(Window* win, int dx, int dy);

#endif