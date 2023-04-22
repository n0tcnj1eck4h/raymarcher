#include "game.hpp"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_timer.h"
#include "graphics/camera.hpp"
#include "graphics/renderer.hpp"
#include "imgui.h"
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

static glm::vec3 eyepos(0, 0, 0);
static glm::vec3 dir = glm::vec3(0, 0, 1);

Game::Game() : m_camera(0.01f, 100.0f, 16.0f / 9.0f, 80.0f) {
  m_frameTime = m_lastFrameTime = SDL_GetTicks64();
  m_renderer.m_raymarcher.setCameraPosition(eyepos);
  m_renderer.m_ddamarcher.setCameraPosition(eyepos);
  m_renderer.m_raymarcher.setCameraDirection(dir);
  m_renderer.m_ddamarcher.setCameraDirection(dir);
  m_hasFocus = true;
}

Game::~Game() {}

void Game::update() {
  m_frameTime = SDL_GetTicks64();
  u64 deltatime = m_frameTime - m_lastFrameTime;
  float deltafloat = deltatime / 1000.0f;

  for (u32 i = 0; i < SDL_NUM_SCANCODES; i++) {
    m_keystates[i] = (Keystate)(m_keystates[i] & 0b01);
  }

  m_lastFrameTime = m_frameTime;

  // eyepos = glm::rotate(eyepos, (float)(m_frameTime) / 100.0f / 3.14f,
  // glm::normalize(glm::vec3(1,1,0)));

  if (!m_hasFocus)
    return;

  glm::vec3 camera_delta(0);
  // std::cout << 1.0f / (deltatime / 1000.0f) << std::endl;

  float speed = 2.f;

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
    camera_delta += deltafloat * glm::vec3(0, 1, 0);
  }
  if (m_keystates[SDL_SCANCODE_LCTRL] & Keystate::PRESSED) {
    camera_delta -= deltafloat * glm::vec3(0, 1, 0);
  }
  if (m_keystates[SDL_SCANCODE_LSHIFT] & Keystate::PRESSED) {
    camera_delta *= 8.f;
  }

  glm::vec3 right = glm::cross(dir, glm::vec3(0, 1, 0));
  right = glm::normalize(right);
  eyepos += right * camera_delta.x * speed;
  eyepos += glm::normalize(dir * glm::vec3(1, 0, 1)) * camera_delta.z * speed;
  eyepos.y += camera_delta.y * speed;

  m_renderer.m_raymarcher.setCameraPosition(eyepos);
  m_renderer.m_ddamarcher.setCameraPosition(eyepos);

  m_camera.moveLocal(camera_delta);
}

void Game::draw() {
  m_renderer.clear();
  // m_renderer.updateView(m_camera);
  // m_renderer.drawCube(glm::mat4(1));
  m_renderer.draw();
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

  if (event.keysym.scancode == SDL_SCANCODE_ESCAPE &&
      event.state == SDL_PRESSED && m_hasFocus) {
    focus(false);
  }
}

bool Game::focus() { return m_hasFocus; }

void Game::focus(bool b) {
  m_hasFocus = b;
  SDL_SetRelativeMouseMode((SDL_bool)b);
  ImGui::GetIO().SetAppAcceptingEvents(!b);
}

void Game::onMouseMotionEvent(const SDL_MouseMotionEvent &event) {
  if (!m_hasFocus)
    return;
  m_camera.rotateY(-event.xrel / 1000.0);
  m_camera.rotateX(event.yrel / 1000.0);

  dir = glm::rotateY(dir, -event.xrel / 1000.0f);
  glm::vec3 right = glm::cross(dir, glm::vec3(0, -1, 0));
  right = glm::normalize(right);
  dir = glm::rotate(dir, event.yrel / 1000.0f, right);

  m_renderer.m_raymarcher.setCameraDirection(dir);
  m_renderer.m_ddamarcher.setCameraDirection(dir);
}

void Game::onMouseButtonEvent(const SDL_MouseButtonEvent &event) {
  if (!m_hasFocus)
    focus(true);
}
