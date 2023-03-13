#pragma once

#include "gl/gl.hpp"
#include <glm/fwd.hpp>
class GLProgram;

class GLUniform {
  const GLProgram &m_program;
  const GLuint m_location;
  GLUniform(const GLProgram &program, const char *location);
  GLUniform(const GLUniform &) = delete;
  GLUniform(GLUniform &&) = delete;
  friend class GLProgram;

public:
  void matrix(const glm::mat4 &matrix);
  void vec3(const glm::vec3 &vector);
};
