#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 1000
#define HEIGHT 700
#define NC_W 250
#define NC_H 175

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

void random_setup() 
{
    for (int i = 0; i < 500; ++i) {
        int x = 100 + get_random_value(50);
        int y = 70 + get_random_value(35);

        change_status(current_gen, x, y, 1);
    }
}

void draw_text_box()
{
    DrawRectangle( 10, 10, 250, 113, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines( 10, 10, 250, 113, RAYWHITE);

    DrawText("Modes:", 20, 20, 10, WHITE);
    DrawText("- R to Make a Random Setup", 40, 40, 10, LIGHTGRAY);
    DrawText("- LEFT_MOUSE to Create Your Own Setup", 40, 60, 10, LIGHTGRAY);
    DrawText("  And then Press Enter", 40, 80, 10, LIGHTGRAY);
    DrawText("- MOUSE_WHEEL to Zoom In or Out", 40, 100, 10, LIGHTGRAY);
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
    */

    // DRAWING BEGINS
    InitWindow(WIDTH, HEIGHT, "Conway's Game of Life");
    SetTargetFPS(20);

    int frame_counter = 0;
    int enter_was_pressed = 0;
    int r_was_pressed = 0;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ WIDTH / 2.0f, HEIGHT / 2.0f };
    camera.offset = (Vector2){ WIDTH / 2.0f, HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

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
        if (IsKeyPressed(KEY_ENTER) && !enter_was_pressed) {
            enter_was_pressed = 1;
        }

        if (enter_was_pressed) {
            next_generation();
            memcpy(current_gen, next_gen, sizeof(current_gen));
        }

        if (IsKeyPressed(KEY_R) && !r_was_pressed) {
            r_was_pressed = 1;
            random_setup();
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
            int gridX = (int)(mousePos.x / 4.0f);
            int gridY = (int)(mousePos.y / 4.0f);

            if (gridX >= 0 && gridX < NC_W && gridY >= 0 && gridY < NC_H) {
                change_status(current_gen, gridX, gridY, 1);
            }
        }
        EndMode2D();

        if (!enter_was_pressed) {
            draw_text_box();
        }

        if (enter_was_pressed) {
            DrawFPS(10, 10);
        }

        EndDrawing();
    }
    
    CloseWindow();
    // DRAWING ENDS

    return 0;
}
