#pragma once
#include "camera.hpp"
#include "glbinding/gl/gl.h"

using namespace gl;

class Renderer {
  Renderer() = delete;
  static GLuint program;
  static GLuint vbo, ibo, solid_box_vao;
  static GLuint viewproj_uniform;
  static GLuint model_uniform;
  static GLuint color_uniform;

public:
  static void init();
  static void deinit();
  static void clear();
  static void draw();
  static void updateView(const Camera &);
  static void drawCube(const glm::mat4 &transform);
};
