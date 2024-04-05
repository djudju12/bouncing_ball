#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef struct {
    int x;
    int y;
} v2i;

typedef struct {
    v2i vel;
    v2i center;
    int radius;
} Ball;

#define TOTAL_BALLS 3
Ball b[TOTAL_BALLS] = {
    {0},
    {0},
    {0},
};
// Ball b1 = {0};
// Ball b2 = {0};
// Ball b3 = {0};

int D_HEIGHT;
int D_WIDTH;
#define inside_display(x,y) (((x) < D_WIDTH && (x) >= 0) && ((y) < D_HEIGHT*2 && (y) >= 0))
char display[2048][2048];

void clear() {
    for (size_t y = 0; y < D_HEIGHT; y++) {
        for (size_t x = 0; x < D_WIDTH; x++) {
            display[y][x] = ' ';
        }
    }
}

void circle(v2i center, int radius) {
    for (int y2 = center.y-radius; y2 <= center.y+radius; y2+=2) {
        for (int x2 = center.x-radius; x2 <= center.x+radius; x2++) {
            if (inside_display(x2,y2)) {
                int dx = x2 - center.x;
                int dy1 = y2 - center.y;
                int dy2 = y2+1 - center.y;
                if ((dx*dx + dy1*dy1) <= radius*radius) {
                    if (inside_display(x2,y2+1) && (dx*dx + dy2*dy2 <= radius*radius)) {
                        display[y2/2][x2] = 'C';
                    } else {
                        display[y2/2][x2] = '\'';
                    }
                } else {
                    if (inside_display(x2,y2+1) && (dx*dx + dy2*dy2 <= radius*radius)) {
                        display[y2/2][x2] = '_';
                    } else {
                        // display[y2/2][x2] = ' ';
                    }
                }
            }
        }
    }
}

void render() {
    for (size_t y = 0; y < D_HEIGHT; y++) {
        for (size_t x = 0; x < D_WIDTH; x++) {
            printf("%c", display[y][x]);
        }
        printf("\n");
    }
}

void draw() {
    for (size_t i = 0; i < TOTAL_BALLS; i++) {
        circle(b[i].center, b[i].radius);
    }
    render();
}

void update() {
    for (size_t i = 0; i < TOTAL_BALLS; i++) {
        if ((b[i].center.x + b[i].radius) >= D_WIDTH || (b[i].center.x - b[i].radius) < 0) {
            b[i].vel.x = -b[i].vel.x;
        }

        if ((b[i].center.y + b[i].radius) > D_HEIGHT*2 || (b[i].center.y - b[i].radius) < 0) {
            b[i].vel.y = -b[i].vel.y;
        }

        printf("b%d ws(%d %d) c(%d %d) v(%2f %2f) %d\n", i+1, D_WIDTH, D_HEIGHT*2,b[i].center.x, b[i].center.y, b[i].vel.x, b[i].vel.y, b[i].radius);
        b[i].center.x += b[i].vel.x;
        b[i].center.y += b[i].vel.y;
    }
}

int main(void) {
    struct winsize sz;
    ioctl( 0, TIOCGWINSZ, &sz );
    D_WIDTH = sz.ws_col;
    D_HEIGHT = sz.ws_row;

    b[0].vel.x = -1;
    b[0].vel.y = 2;
    b[0].center.x = 10;
    b[0].center.y = 10;
    b[0].radius = 10;

    b[1].vel.x = 1;
    b[1].vel.y = -2;
    b[1].center.x = 75;
    b[1].center.y = 50;
    b[1].radius = 10;

    b[2].vel.x = 1;
    b[2].vel.y = 2;
    b[2].center.x = 100;
    b[2].center.y = 55;
    b[2].radius = 10;

    while (1) {
        draw();
        clear();
        update();
        usleep(1000000 * 1/15);
    }
}