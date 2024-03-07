#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#define DEBUG false

#if DEBUG
#define DEBUG_EXP(EXP) EXP
#else
#define DEBUG_EXP(EXP)
#endif

// --------------------------------------
// Declarations
// --------------------------------------
#define PLAYER_1_UP KEY_M
#define PLAYER_1_DOWN KEY_N
#define PLAYER_2_UP KEY_Q
#define PLAYER_2_DOWN KEY_E

#define PLAYER_OFFSET 50
#define PLAYER_SPEED 8.0

#define TIME_TO_WAIT 5
#define MAX_SCORE 5
// #define SPEED_NORMAL 7.5
#define SPEED_NORMAL 0.5

#define min_value(x, y) ((x < y) ? (x) : (y))

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
    bool half;
} Ball;

// --------------------------------------
// Main Loop
// --------------------------------------

void setBallPosition(Ball *ball, bool left) {
    ball->position = (Vector2){(float)(GetScreenWidth() / 2),
                               (float)(GetScreenHeight() / 2)};

    float y_speed = 0;
    if (left) {
        ball->velocity = (Vector2){-4.0f, y_speed};
        ball->half = 0;
    } else {
        ball->velocity = (Vector2){4.0f, y_speed};
        ball->half = 1;
    }

    ball->velocity =
        Vector2Scale(Vector2Normalize(ball->velocity), SPEED_NORMAL);
}

void ballPlayerCollision(Ball *ball, Player *player) {
    ball->velocity.x *= -1;
    if (ball->position.y < player->position.y ||
        ball->position.y > player->position.y + player->size.y) {
        ball->velocity.y *= -1;
    }
    ball->half = !ball->half;
}

int main(void) {
    // Initialization
    int16_t screen_width = 800;
    int16_t screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong!!!!!");

    // Load resources
    Texture2D texBall = LoadTexture("assets/images/ball.png");
    Texture2D texPaddle = LoadTexture("assets/images/paddle.png");
    const int PLAYER_SIZE_X = texPaddle.width;
    const int PLAYER_SIZE_Y = texPaddle.height;
    const int BALL_RADIUS = texBall.height;

    Font font = LoadFont("assets/images/setback.png");

    InitAudioDevice();
    Sound fxStart = LoadSound("assets/audio/start.wav");
    Sound fxBounce = LoadSound("assets/audio/bounce.wav");
    Sound fxPoint = LoadSound("assets/audio/point.wav");

    Music music = LoadMusicStream("assets/audio/background.wav");
    PlayMusicStream(music);

    // Gamestate variables
    GameScreens screen = INTRO;

    int16_t frames_count = 0; // Small number games counter
    bool game_paused = false; // Toggle if the game is paused

    // Initialize players and ball
    // Player players[2] = {0, 0};
    Player players[2] = {{.velocity = (Vector2){0, 0}},
                         {.velocity = (Vector2){0, 0}}};
    Ball ball = {0};

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        switch (screen) {
        case INTRO:
        case ENDING:
            // Set Players and ball variables
            players[0].position = (Vector2){
                PLAYER_OFFSET, (float)(GetScreenHeight() / 2 - PLAYER_SIZE_X)};
            players[0].velocity = (Vector2){0.0f, PLAYER_SPEED};
            players[0].size =
                (Vector2){(float)PLAYER_SIZE_X, (float)PLAYER_SIZE_Y};

            players[1].position =
                (Vector2){screen_width - PLAYER_OFFSET,
                          (float)(GetScreenHeight() / 2 - PLAYER_SIZE_X)};
            players[1].velocity = (Vector2){0.0f, PLAYER_SPEED};
            players[1].size =
                (Vector2){(float)PLAYER_SIZE_X, (float)PLAYER_SIZE_Y};

            setBallPosition(&ball, true);
            ball.radius = (float)BALL_RADIUS;
            frames_count++;
            // Wait 3 seconds
            if (frames_count > 60 * TIME_TO_WAIT) {
                screen = TITLE;
                frames_count = 0;
                PlaySound(fxStart);
            }
            break;
        case TITLE:
            players[0].points = 0;
            players[1].points = 0;
            frames_count++;
            if (IsKeyPressed(KEY_ENTER)) {
                screen = GAMEPLAY;
                PlaySound(fxStart);
            }
            break;
        case GAMEPLAY:
            frames_count = 0;

            if (IsKeyPressed('P')) {
                game_paused = !game_paused;
            }

            if (players[0].points > MAX_SCORE - 1 ||
                players[1].points > MAX_SCORE - 1) {
                screen = ENDING;
            }

            if (!game_paused) {
                // Game Logig
                // Player logic
                if (IsKeyDown(PLAYER_1_UP))
                    players[0].position.y -= players[0].velocity.y;
                if (IsKeyDown(PLAYER_1_DOWN))
                    players[0].position.y += players[0].velocity.y;
                if (IsKeyDown(PLAYER_2_UP))
                    players[1].position.y -= players[1].velocity.y;
                if (IsKeyDown(PLAYER_2_DOWN))
                    players[1].position.y += players[1].velocity.y;

                if (players[0].position.y <= 0)
                    players[0].position.y = 0;
                if (players[0].position.y + players[0].size.y >=
                    (float)GetScreenHeight())
                    players[0].position.y =
                        (float)GetScreenHeight() - players[0].size.y;
                if (players[1].position.y <= 0)
                    players[1].position.y = 1;
                if (players[1].position.y + players[1].size.y >=
                    (float)GetScreenHeight())
                    players[1].position.y =
                        (float)GetScreenHeight() - players[1].size.y;

                players[0].bounds =
                    (Rectangle){players[0].position.x, players[0].position.y,
                                players[0].size.x, players[0].size.y};
                players[1].bounds =
                    (Rectangle){players[1].position.x, players[1].position.y,
                                players[1].size.x, players[1].size.y};

                // Ball logic
                ball.position.x += ball.velocity.x;
                ball.position.y += ball.velocity.y;

                if (ball.position.y + ball.radius >= (float)GetScreenHeight() ||
                    ball.position.y - ball.radius <= 0)
                    ball.velocity.y *= -1;

                // Collision logic
                if (CheckCollisionCircleRec(ball.position, ball.radius,
                                            players[0].bounds) && ball.half == 0) {
                    ballPlayerCollision(&ball, &players[0]);
                    PlaySound(fxBounce);
                }
                if (CheckCollisionCircleRec(ball.position, ball.radius,
                                            players[1].bounds) && ball.half == 1) {
                    ballPlayerCollision(&ball, &players[1]);
                    PlaySound(fxBounce);
                }

                // Point logic
                if (ball.position.x + ball.radius >= (float)GetScreenWidth()) {
                    players[0].points += 1;
                    PlaySound(fxPoint);
                    setBallPosition(&ball, false);
                } else if (ball.position.x - ball.radius <= 0) {
                    players[1].points += 1;
                    PlaySound(fxPoint);
                    setBallPosition(&ball, true);
                }
            }

            break;
        }

        // Refill Music buffer
        UpdateMusicStream(music);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // To display the no. of seconds
        char *seconds = malloc(15 + 1);
        sprintf(seconds, "Wait %d seconds",
                (int)(TIME_TO_WAIT - frames_count / 60));

        switch (screen) {
        case INTRO:
            // DrawText("Intro Screen",
            //          GetScreenWidth() / 2 - MeasureText("Intro Screen", 80) /
            //          2, 350, 80, GRAY);
            DrawTextEx(
                font, "Intro Screen",
                (Vector2){(float)(GetScreenWidth() / 2) -
                              MeasureTextEx(font, "Intro Screen", 80, 2).x / 2,
                          350},
                80, 2, BLUE);
            DrawText(seconds,
                     GetScreenWidth() / 2 - MeasureText(seconds, 20) / 2, 450,
                     20, GRAY);
            break;
        case TITLE:
            // DrawText("PONG", GetScreenWidth() / 2 - MeasureText("PONG", 80) /
            // 2,
            //          350, 80, GRAY);
            DrawTextEx(font, "PONG",
                       (Vector2){(float)(GetScreenWidth() / 2) -
                                     MeasureTextEx(font, "PONG", 80, 2).x / 2,
                                 350},
                       80, 2, BLUE);
            if ((frames_count / 30) % 2 == 0)
                DrawText("Press [ENTER] to Play",
                         GetScreenWidth() / 2 -
                             MeasureText("Press [ENTER] to Play", 20) / 2,
                         450, 20, GRAY);
            break;
        case GAMEPLAY:
            DrawFPS(10, 10);

            // Draw the middle line
            for (int8_t i = 0; i < 10; i++) {
                DrawLine(GetScreenWidth() / 2, i * 100 + 25,
                         GetScreenWidth() / 2, i * 100 + 75, BLACK);
            }

            // Draw players and ball
            // DrawRectangle(players[0].position.x, players[0].position.y,
            //               players[0].size.x, players[0].size.y, BLACK);
            // DrawRectangle(players[1].position.x, players[1].position.y,
            //               players[1].size.x, players[1].size.y, BLACK);
            // DrawCircleV(ball.position, ball.radius, MAROON);
            DrawTextureEx(texPaddle, players[0].position, 0.0f, 1.0f, WHITE);
            DrawTextureEx(texPaddle, players[1].position, 0.0f, 1.0f, WHITE);
            DrawTexture(texBall, (int)(ball.position.x - ball.radius / 2),
                        (int)(ball.position.y - ball.radius / 2), MAROON);

            DrawLine((int)(players[0].position.x),
                     (int)(players[0].position.y + (float)(PLAYER_SIZE_Y / 2)),
                     (int)(ball.position.x), (int)(ball.position.y), BLUE);
            DrawLine((int)(players[1].position.x),
                     (int)(players[1].position.y + (float)(PLAYER_SIZE_Y / 2)),
                     (int)(ball.position.x), (int)(ball.position.y), BLUE);

            // Draw score
            char *score = malloc(15 + 1);
            sprintf(score, "%d", players[0].points);
            DrawText(score,
                     GetScreenWidth() / 2 - MeasureText(score, 80) / 2 - 100, 0,
                     80, GRAY);
            sprintf(score, "%d", players[1].points);
            DrawText(score,
                     GetScreenWidth() / 2 - MeasureText(score, 80) / 2 + 100, 0,
                     80, GRAY);

            // Draw pause message if needed
            if (game_paused) {
                DrawText("GAME PAUSED",
                         GetScreenWidth() / 2 -
                             MeasureText("GAME PAUSED", 80) / 2,
                         350, 80, GRAY);
            }

            break;
        case ENDING:
            if (players[0].points == players[1].points) {
                // DrawText("DRAW",
                //          GetScreenWidth() / 2 - MeasureText("DRAW", 80) / 2,
                //          350, 80, GRAY);
                DrawTextEx(
                    font, "DRAW",
                    (Vector2){(float)(GetScreenWidth() / 2) -
                                  MeasureTextEx(font, "DRAW", 80, 2).x / 2,
                              350},
                    80, 2, BLUE);
            } else if (players[0].points > players[1].points) {
                // DrawText("Player 1 Won",
                //          GetScreenWidth() / 2 -
                //              MeasureText("Player 1 Won", 80) / 2,
                //          350, 80, GRAY);
                DrawTextEx(
                    font, "Player 1 Won",
                    (Vector2){(float)(GetScreenWidth() / 2) -
                                  MeasureTextEx(font, "Player 1 Won", 80, 2).x /
                                      2,
                              350},
                    80, 2, BLUE);
            } else {
                // DrawText("Player 2 Won",
                //          GetScreenWidth() / 2 -
                //              MeasureText("Player 2 Won", 80) / 2,
                //          350, 80, GRAY);
                DrawTextEx(
                    font, "Player 2 Won",
                    (Vector2){(float)(GetScreenWidth() / 2) -
                                  MeasureTextEx(font, "Player 2 Won", 80, 2).x /
                                      2,
                              350},
                    80, 2, BLUE);
            }
            DrawText(seconds,
                     GetScreenWidth() / 2 - MeasureText(seconds, 20) / 2, 450,
                     20, GRAY);
            break;
        }

        free(seconds);

        EndDrawing();
    }

    // Unload textures
    UnloadTexture(texBall);
    UnloadTexture(texPaddle);

    UnloadFont(font);

    UnloadSound(fxStart);
    UnloadSound(fxBounce);
    UnloadSound(fxPoint);

    UnloadMusicStream(music);

    // Deinitialization
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
