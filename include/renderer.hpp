#pragma once
#include "camera.hpp"
#include "gl/buffer.hpp"
#include "gl/vao.hpp"

class Renderer {
  GLuint program;
  GLVertexArray vao;
  GLVertexArrayBuffer vbo;
  GLElementArrayBuffer ibo;
  GLuint viewproj_uniform;
  GLuint model_uniform;
  GLuint color_uniform;

public:
  Renderer();
  ~Renderer();
  void clear();
  void draw();
  void updateView(const Camera &);
  void drawCube(const glm::mat4 &transform);
};
