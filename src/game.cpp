#include "game.hpp"
#include "SDL_events.h"
#include "SDL_scancode.h"
#include "SDL_timer.h"
#include "renderer.hpp"
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

Game::Game() { m_lastframetime = SDL_GetTicks64(); }

Game::~Game() {}

void Game::update() {
  u64 currentframetime = SDL_GetTicks64();
  u64 deltatime = currentframetime - m_lastframetime;
  float deltafloat = deltatime / 1000.0f;

  auto camera_right = glm::cross(glm::vec3(0, -1, 0), m_camera.dir);
  camera_right = glm::normalize(camera_right);
  if (m_keystates[SDL_SCANCODE_W] & Keystate::PRESSED) {
    m_camera.pos += deltafloat * m_camera.dir;
  }
  if (m_keystates[SDL_SCANCODE_S] & Keystate::PRESSED) {
    m_camera.pos -= deltafloat * m_camera.dir;
  }
  if (m_keystates[SDL_SCANCODE_A] & Keystate::PRESSED) {
    m_camera.pos -= deltafloat * camera_right;
  }
  if (m_keystates[SDL_SCANCODE_D] & Keystate::PRESSED) {
    m_camera.pos += deltafloat * camera_right;
  }

  m_camera.updateViewProj();
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
  m_camera.dir = glm::rotateX(m_camera.dir, -(float)event.xrel / 1000.0f);
  m_camera.dir = glm::rotate(
      m_camera.dir, (float)event.yrel / 1000.0f,
      glm::normalize(glm::cross(m_camera.dir, glm::vec3(0, -1, 0))));
}

void Game::onMouseButtonEvent(const SDL_MouseButtonEvent &event) {}
