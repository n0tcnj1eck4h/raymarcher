#pragma once

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_scancode.h"
#include "graphics/camera.hpp"
#include "graphics/renderer.hpp"
#include "imgui.h"
#include "types.hpp"

enum Keystate : u8 {
  RELEASED = 0b00,
  PRESSED = 0b01,
  JUST_RELEASED = 0b10,
  JUST_PRESSEED = 0b11
};

class Game {
  Renderer m_renderer;
  Camera m_camera;
  Keystate m_keystates[SDL_NUM_SCANCODES] = {Keystate::RELEASED};
  u64 m_lastFrameTime, m_frameTime;

public:
  Game();
  ~Game();
  void update();
  void draw();
  void onKeyboardEvent(const SDL_KeyboardEvent &event);
  void onMouseMotionEvent(const SDL_MouseMotionEvent &event);
  void onMouseButtonEvent(const SDL_MouseButtonEvent &event);
};
