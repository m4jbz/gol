#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
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

cell cells[NC_W][NC_H];

cell init_cell(int x, int y)
{
    return (cell) { (Vector2) {x*4.0f, y*4.0f}, BLACK, 0 };
}

int get_random_value(int max)
{
    return random() % max+1;
}

cell change_status(cell c)
{
    if (c.status == 1) {
        c.status = 0;
        c.color = BLACK;
    } else {
        c.status = 1;
        c.color = WHITE;
    }

    return c;
}

int get_neighbours(int x, int  y)
{
    int counter = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }
            
            if (cells[x+i][y+j].status == 1) {
                counter++;
            }
        }
    }

    return counter;
}

cell check_around(cell c, int x, int y)
{
    if (cells[x][y].status == 0) {
        int counter = get_neighbours(x, y);

        if (counter == 3) {
            c.status = 1;
            c.color = WHITE;
        }
    } else {
        int counter = get_neighbours(x, y);

        if (counter < 2 || counter > 3) {
            c.status = 0;
            c.color = BLACK;
        }
    }

    return c;
}

int main()
{
    srandom(time(NULL));

    // INIT ALL CELLS AS DEAD
    for (int i = 0; i < NC_W; ++i) {
        for (int j = 0; j < NC_H; ++j) {
            cells[i][j] = init_cell(i, j);
        }
    }

    // PSEUDO-RANDOM POSITIONS FOR ALIVE CELLS
    for (int i = 0; i < 200; ++i) {
        int x = 75 + get_random_value(50);
        int y = 56 + get_random_value(37);

        cells[x][y] = change_status(cells[x][y]);
    }

    // DRAWING BEGINS
    InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RED);

        // CELLS DRAWING
        for (int i = 0; i < NC_W; ++i) {
            for (int j = 0; j < NC_H; ++j) {
                cells[i][j] = check_around(cells[i][j], i, j);
                DrawRectangleV(cells[i][j].pos, SIZE, cells[i][j].color);
            }
        }

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    // DRAWING ENDS

    return 0;
}
