#include "SDL2/SDL.h"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_video.h"
#include "config.hpp"
#include "game.hpp"
#include "glbinding/glbinding.h"
#include <cassert>
#include <iostream>

int main(int, const char **) {
  assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);

  auto window =
      SDL_CreateWindow("xd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  if (!window) {
    std::cerr << SDL_GetError() << std::endl;
    assert(window);
  }

  auto gl_context = SDL_GL_CreateContext(window);
  if (!gl_context) {
    std::cerr << SDL_GetError() << std::endl;
    assert(gl_context);
  }

  glbinding::initialize(nullptr);

  // SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_ShowWindow(window);

  {
    Game game;
    SDL_Event event;
    while (true) {
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
          goto shutdown;
        case SDL_MOUSEMOTION:
          game.onMouseMotionEvent(event.motion);
          break;
        case SDL_KEYDOWN:
          game.onKeyboardEvent(event.key);
          break;
        case SDL_KEYUP:
          game.onKeyboardEvent(event.key);
          break;
        }
      }

      game.update();
      game.draw();
      SDL_GL_SwapWindow(window);
    }
  }

shutdown:
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
