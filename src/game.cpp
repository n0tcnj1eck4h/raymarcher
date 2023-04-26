#include "game.hpp"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_timer.h"
#include "config.hpp"
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
  // m_renderer.m_ddamarcher.setCameraPosition(eyepos);
  m_renderer.m_raymarcher.setCameraDirection(dir);
  // m_renderer.m_ddamarcher.setCameraDirection(dir);
  m_hasFocus = true;

  m_renderer.viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

  if (ImGui::Begin("Scene", nullptr))
    ;
  ImGui::End();

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
  // m_renderer.m_ddamarcher.setCameraPosition(eyepos);

  m_camera.moveLocal(camera_delta);
}

void Game::draw() {
  m_renderer.clear();
  m_renderer.draw();

  const float PAD = 10.0f;
  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImVec2 work_pos = viewport->WorkPos;
  ImVec2 work_size = viewport->WorkSize;
  ImVec2 window_pos, window_pos_pivot;
  window_pos.x = (work_pos.x + PAD);
  window_pos.y = (work_pos.y + PAD);
  window_pos_pivot.x = 0.0f;
  window_pos_pivot.y = 0.0f;
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

  ImGuiIO &io = ImGui::GetIO();
  ImGui::SetNextWindowBgAlpha(0.35f);
  if (ImGui::Begin("Example: Simple overlay", nullptr,
                   ImGuiWindowFlags_NoDecoration |
                       ImGuiWindowFlags_AlwaysAutoResize |
                       ImGuiWindowFlags_NoFocusOnAppearing |
                       ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove)) {
    if (ImGui::IsMousePosValid())
      ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    else
      ImGui::Text("Mouse Position: <invalid>");
  }
  ImGui::Text("Camera Position: (%.1f,%.1f,%.1f)", eyepos.x, eyepos.y,
              eyepos.z);
  ImGui::Text("Camera Direction: (%.1f,%.1f,%.1f)", dir.x, dir.y, dir.z);
  ImGui::End();
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
  SDL_SetRelativeMouseMode(b ? SDL_TRUE : SDL_FALSE);

  ImGuiIO &io = ImGui::GetIO();
  io.SetAppAcceptingEvents(!b);
  io.ClearInputKeys();
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
  // m_renderer.m_ddamarcher.setCameraDirection(dir);
}

void Game::onMouseButtonEvent(const SDL_MouseButtonEvent &event) {
  if (!m_hasFocus)
    focus(true);
}

void Game::onWindowResize(i64 width, i64 height) {
  m_renderer.viewport(0, 0, width, height);
}
