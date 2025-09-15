#include "raylib.h"

constexpr int screenWidth = 800;
constexpr int screenHeight = 600;

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(LIGHTGRAY);
        DrawText("Congrats! You created your first window!", 190, 200, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
}