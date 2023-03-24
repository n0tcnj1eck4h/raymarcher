#pragma once

#include "gl/program.hpp"
#include <glm/fwd.hpp>

class DDAMarcherShader : public GLProgram {
  GLUniform m_dirUniform;
  GLUniform m_eyeUniform;

  public:
    DDAMarcherShader();
    void setCameraPosition(const glm::vec3& pos);
    void setCameraDirection(const glm::vec3& dir);
};
