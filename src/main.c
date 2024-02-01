#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------
// Types
// --------------------------------------

typedef enum GameScreens { INTRO, TITLE, GAMEPLAY, ENDING } GameScreens;

// --------------------------------------
// Main Loop
// --------------------------------------

int main(void) {
    // Initialization
    int16_t screen_width = 800;
    int16_t screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong!!!!!");

    // Gamestate variables
    GameScreens screen = INTRO;

    int16_t frames_count = 0; // Small number games counter
    char game_result = -1;    // 0 - loose, 1 - win, -1 - TBD
    bool game_paused = false; // Toggle if the game is paused

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        switch (screen) {
        case INTRO:
        case ENDING:
            frames_count++;
            // Wait 3 seconds
            if (frames_count > 180) {
                screen = TITLE;
                frames_count = 0;
            }
            break;
        case TITLE:
            if (IsKeyPressed(KEY_ENTER))
                screen = GAMEPLAY;
            break;
        case GAMEPLAY:
            if (IsKeyPressed(KEY_ENTER))
                screen = ENDING;
            break;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // To display the no. of seconds
        char *seconds = malloc(15 + 1);
        sprintf(seconds, "Wait %d seconds", (int)(3 - frames_count / 60));

        switch (screen) {
        case INTRO:
            DrawText("Intro Screen", 150, 350, 80, GRAY);
            DrawText(seconds, 330, 450, 20, GRAY);
            break;
        case TITLE:
            DrawText("PONG", 300, 350, 80, GRAY);
            DrawText("Press ENTER to Play", 300, 450, 20, GRAY);
            break;
        case GAMEPLAY:
            DrawText("Test Text", 20, 20, 40, GRAY);
            break;
        case ENDING:
            DrawText("Player X Won", 125, 350, 80, GRAY);
            DrawText(seconds, 330, 450, 20, GRAY);
            break;
        }

        free(seconds);

        EndDrawing();
    }

    // Deinitialization
    CloseWindow();

    return 0;
}
