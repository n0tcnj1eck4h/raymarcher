#include "gl/uniform.hpp"
#include "gl/program.hpp"
#include <glm/gtc/type_ptr.hpp>

template <> void GLUniform<glm::mat4>::set(const glm::mat4 &value) {
  m_program.use();
  glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(value));
}

template <> void GLUniform<glm::vec2>::set(const glm::vec2 &value) {
  m_program.use();
  glUniform2fv(m_location, 1, glm::value_ptr(value));
}

template <> void GLUniform<glm::vec3>::set(const glm::vec3 &value) {
  m_program.use();
  glUniform3fv(m_location, 1, glm::value_ptr(value));
}

template <> void GLUniform<GLfloat>::set(const GLfloat &value) {
  m_program.use();
  glUniform1f(m_location, value);
}

template <> void GLUniform<GLint>::set(const GLint &value) {
  m_program.use();
  glUniform1i(m_location, value);
}
