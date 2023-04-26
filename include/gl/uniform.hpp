#pragma once

#include "gl/gl.hpp"
#include <glm/fwd.hpp>
class GLProgram;

class GLUniform {
  const GLProgram &m_program;
  const GLuint m_location;

public:
  GLUniform(const GLProgram &program, const char *location);
  void matrix(const glm::mat4 &matrix);
  void vec3(const glm::vec3 &vector);
  void scalar(float scalar);
};
