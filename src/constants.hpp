#pragma once

#include <raylib.h>

constexpr float FPS = 144.0f;
constexpr float DT = 1.0f / FPS;

constexpr float PLAYER_SPEED = 100.0f;

/** Width of a mirror in game units. */
constexpr float MIRROR_WIDTH = 16.0f;

/** The length of the laser when rendered, in game units */
constexpr float LASER_LEN = 16.0f;
constexpr float LASER_SPEED = 800.0f;
constexpr float MIN_LASER_LENGTH = 400.0f;

/** Lower makes the camera snap to the desired position slower, makes the
 * camera feel 'looser', as if it were on a much looser elastic band. A value
 * of 1 will make the camera perfectly track the desired position. A value of 0
 * will stop the camera moving. */
constexpr float CAMERA_SNAP_SPEED = 0.05;

/** Basic enemy movement speed */
constexpr float ENEMY_BASIC_WALK_SPEED = 50.0f;

/* UI constants defining the size / color of the UI widgets: */

/** Height of the picker (displayed along bottom of screen) */
constexpr float PICKER_HEIGHT = 128.0;
/** Scale of tiles in the picker */
constexpr float PICKER_TILE_SIZE = 32.0;
/** Spacing between tiles in the picker */
constexpr float PICKER_SPACING = 16.0;
/** The 'edge color' used across the UI to accentuate UI elements and make them
 * stick out against other screen elements. */
constexpr Color UI_EDGE_COL{0, 200, 255, 255};
