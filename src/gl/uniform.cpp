#include "gl/uniform.hpp"
#include "gl/program.hpp"
#include <glm/gtc/type_ptr.hpp>

GLUniform::GLUniform(const GLProgram &program, const char *location)
    : m_program(program),
      m_location(glGetUniformLocation(program.m_id, location)) {}

void GLUniform::matrix(const glm::mat4 &matrix) {
  glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void GLUniform::vec3(const glm::vec3 &vector) {
  glUniform3fv(m_location, 1, glm::value_ptr(vector));
}

void GLUniform::scalar(float scalar) { glUniform1f(m_location, scalar); }
