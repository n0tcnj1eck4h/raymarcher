#pragma once

#include "SDL_events.h"
#include "SDL_scancode.h"
#include "camera.hpp"
#include "renderer.hpp"
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
  u64 m_lastframetime, m_frametime;

public:
  Game();
  ~Game();
  void update();
  void draw();
  void onKeyboardEvent(const SDL_KeyboardEvent &event);
  void onMouseMotionEvent(const SDL_MouseMotionEvent &event);
  void onMouseButtonEvent(const SDL_MouseButtonEvent &event);
};
