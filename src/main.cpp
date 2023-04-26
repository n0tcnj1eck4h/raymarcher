#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_video.h"
#include "config.hpp"
#include "game.hpp"
#include "glbinding/glbinding.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include <cassert>
#include <glbinding/ProcAddress.h>
#include <iostream>

int main(int, const char **) {
  assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);

  auto window = SDL_CreateWindow(
      "COTMVE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
      WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    std::cerr << SDL_GetError() << std::endl;
    assert(window);
  }

  auto gl_context = SDL_GL_CreateContext(window);
  if (!gl_context) {
    std::cerr << SDL_GetError() << std::endl;
    assert(gl_context);
  }

  // SDL_GL_SetSwapInterval(0);

  glbinding::initialize(
      reinterpret_cast<glbinding::ProcAddress (*)(const char *)>(
          SDL_GL_GetProcAddress));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 330");

  ImGuiIO &io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  SDL_ShowWindow(window);

  static bool show_demo_window = true;

  {
    Game game;
    SDL_Event event;
    game.focus(true);
    while (true) {
      while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
          goto shutdown;
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
          if (!io.WantCaptureMouse)
            game.onMouseButtonEvent(event.button);
          break;
        case SDL_MOUSEMOTION:
          if (!io.WantCaptureMouse)
            game.onMouseMotionEvent(event.motion);
          break;
        case SDL_KEYDOWN:
          if (!io.WantCaptureKeyboard)
            game.onKeyboardEvent(event.key);
          break;
        case SDL_KEYUP:
          if (!io.WantCaptureKeyboard)
            game.onKeyboardEvent(event.key);
          break;
        case SDL_WINDOWEVENT:
          switch (event.window.event) {
          case SDL_WINDOWEVENT_RESIZED:
            game.onWindowResize(event.window.data1, event.window.data2);
            break;
          }
          break;
        }
      }

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();

      if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

      game.update();
      game.draw();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      SDL_GL_SwapWindow(window);
    }
  }

shutdown:
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
