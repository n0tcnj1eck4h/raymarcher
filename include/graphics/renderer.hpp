#pragma once
#include "graphics/camera.hpp"
#include "gl/buffer.hpp"
#include "gl/vao.hpp"
#include "graphics/shaders/ddamarcher.hpp"
#include "graphics/shaders/rasterizer.hpp"
#include "graphics/shaders/raymarcher.hpp"

class Renderer {
  GLVertexArray m_vao;
  GLVertexArrayBuffer m_vbo;
  GLElementArrayBuffer m_ibo;

  GLVertexArray m_vao2;
  GLVertexArrayBuffer m_vbo2;
  GLElementArrayBuffer m_ibo2;
  GLShaderStorageBuffer m_ssbo;

public:
  Renderer();
  void clear();
  void draw();

  RasterizerShader m_rasterizer;
  RaymarcherShader m_raymarcher;
  DDAMarcherShader m_ddamarcher;
};
