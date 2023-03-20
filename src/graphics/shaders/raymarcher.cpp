#include "graphics/shaders/raymarcher.hpp"
#include "graphics/shaders.hpp"

RaymarcherShader::RaymarcherShader()
    : GLProgram(vertex_shader_source2, fragment_shader_source2),
      m_dirUniform(getUniform("dir")), m_eyeUniform(getUniform("eye")) {}

void RaymarcherShader::setCameraDirection(const glm::vec3 &dir) {
  use();
  m_dirUniform.vec3(dir);
}

void RaymarcherShader::setCameraPosition(const glm::vec3 &pos) {
  use();
  m_eyeUniform.vec3(pos);
}
