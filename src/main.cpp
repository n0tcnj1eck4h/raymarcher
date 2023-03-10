#include "SDL2/SDL_mouse.h"
#include "SDL_events.h"
#include "game.hpp"
#include "glbinding/glbinding.h"
#include "renderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <cassert>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

int main(int, const char **) {
  assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

  auto window =
      SDL_CreateWindow("xd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1080, 720, SDL_WINDOW_OPENGL);
  assert(window);

  auto gl_context = SDL_GL_CreateContext(window);
  assert(gl_context);

  glbinding::initialize(nullptr);
  Renderer::init();

  SDL_SetRelativeMouseMode(SDL_TRUE);
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
  Renderer::deinit();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
