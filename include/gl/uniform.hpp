#pragma once

#include "gl/gl.hpp"
#include "gl/program.hpp"
#include "types.hpp"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

template <typename T> class GLUniform {
  const GLProgram &m_program;
  const GLuint m_location;

public:
  GLUniform(const GLProgram &program, const char *location)
      : m_program(program),
        m_location(glGetUniformLocation(program.ID(), location)) {}

  void set(const T &value);
};

//
// void GLUniform::vec3(const glm::vec3 &vector) {
//   glUniform3fv(m_location, 1, glm::value_ptr(vector));
// }
//
// void GLUniform::float32(float scalar) { glUniform1f(m_location, scalar); }
// void GLUniform::int32(i32 scalar) { glUniform1i(m_location, scalar); }
