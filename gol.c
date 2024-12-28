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
    /*
    for (int i = 0; i < 500; ++i) {
        int x = 75 + get_random_value(50);
        int y = 56 + get_random_value(37);

        change_status(current_gen, x, y, 1);
    }
    */

    // DRAWING BEGINS
    InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");
    SetTargetFPS(20);

    int frame_counter = 0;
    int enter_was_pressed = 0;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ WIDTH / 2.0f, HEIGHT / 2.0f };
    camera.offset = (Vector2){ WIDTH / 2.0f, HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        // ZOOM CONTROL
        if (GetMouseWheelMove() != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            
            // UPDATE CAMERA ZOOM (CLAMPING ZOOM VALUE)
            camera.zoom += (GetMouseWheelMove() * 0.5f);
            if (camera.zoom < 0.1f) camera.zoom = 0.1f;
            if (camera.zoom > 5.0f) camera.zoom = 5.0f;

            // GET THE NEW WORLD POINT UNDER MOUSE AFTER ZOOM
            Vector2 mouseWorldPosNew = GetScreenToWorld2D(GetMousePosition(), camera);

            // UPDATE CAMERA TARGET TO ZOOM INTO MOUSE POSITION
            camera.target = (Vector2){
                camera.target.x + (mouseWorldPos.x - mouseWorldPosNew.x),
                camera.target.y + (mouseWorldPos.y - mouseWorldPosNew.y)
            };
        }

        // Update the grid
        if (IsKeyPressed(KEY_ENTER)) {
            enter_was_pressed = 1;
        }

        if (enter_was_pressed) {
            next_generation();
            memcpy(current_gen, next_gen, sizeof(current_gen));
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        // Draw the cells
        for (int i = 0; i < NC_W; i++) {
            for (int j = 0; j < NC_H; j++) {
                DrawRectangleV(current_gen[i][j].pos, SIZE, current_gen[i][j].color);
            }
        }

        // Mouse interaction
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            int gridX = (int)(mousePos.x / 4);
            int gridY = (int)(mousePos.y / 4);

            if (gridX >= 0 && gridX < NC_W && gridY >= 0 && gridY < NC_H) {
                change_status(current_gen, gridX, gridY, 1);
            }
        }

        EndMode2D();

        DrawFPS(10, 10);
        EndDrawing();
    }
    
    CloseWindow();
    // DRAWING ENDS

    return 0;
}
