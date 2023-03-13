#include "game.hpp"
#include "SDL_events.h"
#include "SDL_scancode.h"
#include "SDL_timer.h"
#include "camera.hpp"
#include "renderer.hpp"
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

Game::Game() : m_camera(0.01f, 100.0f, 16.0f / 9.0f, 80.0f) {
  m_lastframetime = SDL_GetTicks64();
}

Game::~Game() {}

void Game::update() {
  u64 currentframetime = SDL_GetTicks64();
  u64 deltatime = currentframetime - m_lastframetime;
  float deltafloat = deltatime / 1000.0f;

  glm::vec3 camera_delta(0);

  if (m_keystates[SDL_SCANCODE_W] & Keystate::PRESSED) {
    camera_delta += deltafloat * glm::vec3(0, 0, 1);
  }
  if (m_keystates[SDL_SCANCODE_S] & Keystate::PRESSED) {
    camera_delta -= deltafloat * glm::vec3(0, 0, 1);
  }
  if (m_keystates[SDL_SCANCODE_A] & Keystate::PRESSED) {
    camera_delta -= deltafloat * glm::vec3(1, 0, 0);
  }
  if (m_keystates[SDL_SCANCODE_D] & Keystate::PRESSED) {
    camera_delta += deltafloat * glm::vec3(1, 0, 0);
  }
  if (m_keystates[SDL_SCANCODE_SPACE] & Keystate::PRESSED) {
    camera_delta -= deltafloat * glm::vec3(0, -1, 0);
  }
  if (m_keystates[SDL_SCANCODE_LCTRL] & Keystate::PRESSED) {
    camera_delta += deltafloat * glm::vec3(0, -1, 0);
  }
  if (m_keystates[SDL_SCANCODE_LSHIFT] & Keystate::PRESSED) {
    camera_delta *= 2.f;
  }

  m_camera.moveLocal(camera_delta);

  for (u32 i = 0; i < SDL_NUM_SCANCODES; i++) {
    m_keystates[i] = (Keystate)(m_keystates[i] & 0b01);
  }

  m_lastframetime = currentframetime;
}

void Game::draw() {
  m_renderer.clear();
  m_renderer.updateView(m_camera);
  m_renderer.drawCube(glm::mat4(1.0f));
}

void Game::onKeyboardEvent(const SDL_KeyboardEvent &event) {
  if (event.state == SDL_PRESSED) {
    if (!(m_keystates[event.keysym.scancode] & 1)) {
      m_keystates[event.keysym.scancode] = Keystate::JUST_PRESSEED;
    }
  } else if (event.state == SDL_RELEASED) {
    if (m_keystates[event.keysym.scancode] & 1) {
      m_keystates[event.keysym.scancode] = Keystate::JUST_RELEASED;
    }
  }
}

void Game::onMouseMotionEvent(const SDL_MouseMotionEvent &event) {
  m_camera.rotateY(-event.xrel / 1000.0);
  m_camera.rotateX(event.yrel / 1000.0);
}

void Game::onMouseButtonEvent(const SDL_MouseButtonEvent &event) {}
