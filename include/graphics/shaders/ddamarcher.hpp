#pragma once

#include "gl/program.hpp"
#include "gl/uniform.hpp"
#include <glm/fwd.hpp>

class DDAMarcherShader : public GLProgram {
public:
  DDAMarcherShader();
  GLUniform<glm::vec3> directionUniform;
  GLUniform<glm::vec3> eyeposUniform;
};
