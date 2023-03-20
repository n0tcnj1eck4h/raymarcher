#pragma once

#include "gl/program.hpp"
#include <glm/fwd.hpp>

class RasterizerShader : public GLProgram {
  GLUniform m_viewProjUniform;
  GLUniform m_colorUniform;
  GLUniform m_modelUniform;

  public:
    RasterizerShader();
    void setViewProjMatrix(const glm::mat4& mat);
    void setModelMatrix(const glm::mat4& mat);
    void setDrawColor(const glm::vec3& col);
};
