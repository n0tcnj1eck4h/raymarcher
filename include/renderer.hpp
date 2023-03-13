#pragma once
#include "camera.hpp"
#include "gl/buffer.hpp"
#include "gl/program.hpp"
#include "gl/uniform.hpp"
#include "gl/vao.hpp"

class Renderer {
  GLProgram program;
  GLVertexArray vao;
  GLVertexArrayBuffer vbo;
  GLElementArrayBuffer ibo;
  GLUniform viewproj_uniform;
  GLUniform model_uniform;
  GLUniform color_uniform;

public:
  Renderer();
  void clear();
  void draw();
  void updateView(const Camera &);
  void drawCube(const glm::mat4 &transform);
};
