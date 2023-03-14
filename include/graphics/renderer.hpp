#pragma once
#include "graphics/camera.hpp"
#include "gl/buffer.hpp"
#include "gl/program.hpp"
#include "gl/uniform.hpp"
#include "gl/vao.hpp"

class Renderer {
  GLProgram m_program;
  GLVertexArray m_vao;
  GLVertexArrayBuffer m_vbo;
  GLElementArrayBuffer m_ibo;
  GLUniform m_viewprojUniform;
  GLUniform m_modelUniform;
  GLUniform m_colorUniform;

public:
  Renderer();
  void clear();
  void draw();
  void updateView(const Camera &);
  void drawCube(const glm::mat4 &transform);
};
