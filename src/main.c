#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG true

#ifdef DEBUG
#define DEBUG_EXP(EXP) EXP
#else
#define DEBUG_EXP(EXP)
#endif /* ifdef  DEBUG */

// --------------------------------------
// Declarations
// --------------------------------------

#define PLAYER_OFFSET 50

// --------------------------------------
// Types
// --------------------------------------

typedef enum GameScreens { INTRO, TITLE, GAMEPLAY, ENDING } GameScreens;

// Object types
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    Rectangle bounds;
    int points;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
} Ball;

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
    DEBUG_EXP(screen = GAMEPLAY;)

    int16_t frames_count = 0; // Small number games counter
    char game_result = -1;    // 0 - loose, 1 - win, -1 - TBD
    bool game_paused = false; // Toggle if the game is paused

    // Initialize players and ball
    Player players[2] = {0, 0};
    players[0].position = (Vector2){ PLAYER_OFFSET, screen_height/2 - 10 };
    players[0].velocity = (Vector2){ 8.0f, 0.0f };
    players[0].size = (Vector2){ 50, 20 };
    players[0].points = 0;

    players[1].position = (Vector2){ screen_width - PLAYER_OFFSET, screen_height/2 - 10 };
    players[1].velocity = (Vector2){ 8.0f, 0.0f };
    players[1].size = (Vector2){ 50, 20 };
    players[1].points = 0;

    Ball ball = {0};

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
