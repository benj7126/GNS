#include <iostream>
#include <raylib.h>
//#include "../include/FontManager.h"
#include "../include/ApplicationManager.h"

int main(void) {
    // const int screenWidth = 800;
    // const int screenHeight = 450;

    ApplicationManager AM{};

    InitWindow(0, 0, "raylib [core] example - basic window");
    SetTargetFPS(60);
    ToggleFullscreen();

    int display = GetCurrentMonitor();
    AM.H = GetMonitorHeight(display);
    AM.W = GetMonitorWidth(display);

    while (!WindowShouldClose())
    {
        // Update
        //stateHandler.UpdateState();
        //TextHandler::Update();

        AM.Update();

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //stateHandler.DrawState();

        int fontSize = 25;
        // DrawTextEx(FontManager::GetFont("Vera", fontSize, 0), "Congrats! You created your first window!", {190, 200-20}, fontSize, 0, BLACK);
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        AM.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}