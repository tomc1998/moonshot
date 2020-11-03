#include <raylib.h>

int main(void) {
  int screen_w = 1600;
  int screen_h = 900;
  InitWindow(screen_w, screen_h, "window_me");
  SetTargetFPS(60);

  Vector2 player_pos{300, 300};

  while (!WindowShouldClose()) {
    // Update player pos
    if (IsKeyDown(KEY_RIGHT)) player_pos.x += 2.0f;
    if (IsKeyDown(KEY_LEFT)) player_pos.x -= 2.0f;
    if (IsKeyDown(KEY_UP)) player_pos.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN)) player_pos.y += 2.0f;

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Draw player
    DrawRectangleV(player_pos, {64, 64}, BLACK);
    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

