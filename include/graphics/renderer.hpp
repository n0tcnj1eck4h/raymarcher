#pragma once
#include "graphics/camera.hpp"
#include "gl/buffer.hpp"
#include "gl/program.hpp"
#include "gl/uniform.hpp"
#include "gl/vao.hpp"

class Renderer {
  GLProgram m_program;
  GLProgram m_program2;

  GLVertexArray m_vao;
  GLVertexArrayBuffer m_vbo;
  GLElementArrayBuffer m_ibo;

  GLVertexArray m_vao2;
  GLVertexArrayBuffer m_vbo2;
  GLElementArrayBuffer m_ibo2;
  GLShaderStorageBuffer m_ssbo;

  GLUniform m_viewprojUniform;
  GLUniform m_modelUniform;
  GLUniform m_colorUniform;

public:
  Renderer();
  void clear();
  void drawScreen();
  void updateView(const Camera &);
  void drawCube(const glm::mat4 &transform);
};
