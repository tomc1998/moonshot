#pragma once

#include "editor_state.hpp"
#include "screen.hpp"
#include <raylib.h>

struct EditorScreen : public Screen {
  EditorState state;
  Camera2D camera;

  EditorScreen();

  void on_mount() override;
  void on_frame(ScreenStack &stack) override;

  /** Serialise & test the level */
  void test(ScreenStack &stack);
};
