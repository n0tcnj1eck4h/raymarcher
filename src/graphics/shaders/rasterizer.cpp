#include "graphics/shaders/rasterizer.hpp"
#include "gl/uniform.hpp"
#include "graphics/shaders.hpp"

RasterizerShader::RasterizerShader()
    : GLProgram(vertex_shader_source, fragment_shader_source),
      m_viewProjUniform(getUniform("viewProj")),
      m_colorUniform(getUniform("color")), m_modelUniform(getUniform("model")) {
}

void RasterizerShader::setDrawColor(const glm::vec3 &col) {
  use();
  m_colorUniform.vec3(col);
}

void RasterizerShader::setModelMatrix(const glm::mat4 &mat) {
  use();
  m_modelUniform.matrix(mat);
}

void RasterizerShader::setViewProjMatrix(const glm::mat4 &mat) {
  use();
  m_viewProjUniform.matrix(mat);
}
