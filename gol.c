#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define NC_W 200
#define NC_H 150

#define SIZE (Vector2) {4.0f, 4.0f}

typedef struct {
    Vector2 pos;
    Color color;
    int status;
} cell;

cell current_gen[NC_W][NC_H];
cell next_gen[NC_W][NC_H];

cell init_cell(int x, int y)
{
    return (cell) { (Vector2) {x*4.0f, y*4.0f}, BLACK, 0 };
}

int get_random_value(int max)
{
    return random() % max+1;
}

void change_status(cell grid[NC_W][NC_H], int x, int y, int new_status)
{
    if (new_status == 1) {
        grid[x][y].color = WHITE;
        grid[x][y].status = 1;
    }

    if (new_status == 0) {
        grid[x][y].color = BLACK;
        grid[x][y].status = 0;
    }
}

int get_neighbours(cell grid[NC_W][NC_H], int x, int  y)
{
    int counter = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }
            
            if (grid[x+i][y+j].status == 1) {
                counter++;
            }
        }
    }

    return counter;
}

void next_generation()
{
    for (int i = 0; i < NC_W; i++) {
        for (int j = 0; j < NC_H; j++) {
            int neighbors = get_neighbours(current_gen, i, j);
            if (current_gen[i][j].status == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    change_status(next_gen, i, j, 0);
                } else {
                    change_status(next_gen, i, j, 1);
                }
            } else {
                if (neighbors == 3) {
                    change_status(next_gen, i, j, 1);
                } else {
                    change_status(next_gen, i, j, 0);
                }
            }
        }
    }
}

void init_alive_current_gen()
{
    current_gen[99][74].color = WHITE;
    current_gen[99][74].status = 1;

    current_gen[100][74].color = WHITE;
    current_gen[100][74].status = 1;

    current_gen[101][74].color = WHITE;
    current_gen[101][74].status = 1;

    current_gen[99][75].color = WHITE;
    current_gen[99][75].status = 1;

    current_gen[101][75].color = WHITE;
    current_gen[101][75].status = 1;

    current_gen[99][76].color = WHITE;
    current_gen[99][76].status = 1;

    current_gen[101][76].color = WHITE;
    current_gen[101][76].status = 1;
}

int main()
{
    srandom(time(NULL));

    // INIT ALL CURRENT_GEN AND NEXT_GEN AS DEAD
    for (int i = 0; i < NC_W; ++i) {
        for (int j = 0; j < NC_H; ++j) {
            current_gen[i][j] = init_cell(i, j);
            next_gen[i][j] = init_cell(i, j);
        }
    }

    // PSEUDO-RANDOM POSITIONS FOR ALIVE CURRENT_GEN
    for (int i = 0; i < 500; ++i) {
        int x = 75 + get_random_value(50);
        int y = 56 + get_random_value(37);

        change_status(current_gen, x, y, 1);
    }

    // INITIALIZE ALIVE CELLS AS DESIRED
    // init_alive_current_gen();

    // DRAWING BEGINS
    InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");
    SetTargetFPS(60);

    int frame_counter = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        // CURRENT_GEN DRAWING
        for (int i = 0; i < NC_W; i++) {
            for (int j = 0; j < NC_H; j++) {
                DrawRectangleV(current_gen[i][j].pos, SIZE, current_gen[i][j].color);
            }
        }

        // UPDATE CURRENT_GEN
        next_generation();
        memcpy(current_gen, next_gen, sizeof(current_gen));

        DrawFPS(10, 10);

        // SCREENSHOT SECTION FOR MAKING GIFS WITH FFMPEG
        char filename[64];
        snprintf(filename, 64, "frames/frame%03d.png", frame_counter++); // Name frames sequentially
        TakeScreenshot(filename);
        
        EndDrawing();
    }

    CloseWindow();
    // DRAWING ENDS

    return 0;
}
