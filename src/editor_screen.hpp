#pragma once

#include "editor_state.hpp"
#include "screen.hpp"
#include <raylib.h>

/* Just some UI constants defining the size / color of the UI widgets: */

/** Height of the picker (displayed along bottom of screen) */
constexpr float PICKER_HEIGHT = 128.0;
/** Scale of tiles in the picker */
constexpr float PICKER_SCALE = 2.0;
/** Spacing between tiles in the picker */
constexpr float PICKER_SPACING = 16.0;
/** The 'edge color' used across the UI to accentuate UI elements and make them
 * stick out against other screen elements. */
constexpr Color UI_EDGE_COL{0, 200, 255, 255};

struct EditorScreen : public Screen {
  EditorState state;
  Camera2D camera;

  EditorScreen();

  void on_mount() override;
  void on_frame() override;
};
