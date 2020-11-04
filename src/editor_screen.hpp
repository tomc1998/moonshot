#include "globals.hpp"
#include "screen.hpp"
#include <raylib.h>

struct EditorScreen : public Screen {
  void on_frame() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(100, 100, 16, 16, BLUE);
    DrawFPS(10, screen_h - 26);
    EndDrawing();
  }
};
