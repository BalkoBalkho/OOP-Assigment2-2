#include "raylib.h"
#include "Classes.hpp"
#include "Units.hpp"
#include "JsonRegistry.h"
#include "gamestuff.h"
#include "Drawing.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main()
{
    GameState gs;
    Drawing::ss;
    Drawing::dl;


    InitWindow(1366, 768, "Parasite");
    SetTargetFPS(60);

    bool showMessageBox = false;
    while (!WindowShouldClose())
    {
    BeginDrawing();
        Monsterjsonregistry mj;

        mj.get_monster(1, {1,2}, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}