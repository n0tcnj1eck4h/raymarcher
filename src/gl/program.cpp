#include "gl/program.hpp"
#include "gl/uniform.hpp"
#include <glbinding/gl/types.h>
#include <glbinding/gl46core/enum.h>
#include <glbinding/gl46core/functions.h>
#include <iostream>

GLuint createShader(const char *source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

#ifdef DEBUG
  GLint len, success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (!success) {
    auto log = new char[len];
    glGetShaderInfoLog(shader, len, NULL, log);
    std::cout << log << std::endl;
    delete[] log;
  }
#endif

  return shader;
}

GLProgram::GLProgram(const char *vert_source, const char *frag_source) {
  m_id = glCreateProgram();
  GLuint vert, frag;
  vert = createShader(vert_source, GL_VERTEX_SHADER);
  frag = createShader(frag_source, GL_FRAGMENT_SHADER);
  glAttachShader(m_id, vert);
  glAttachShader(m_id, frag);
  glLinkProgram(m_id);

  glDeleteShader(vert);
  glDeleteShader(frag);

#ifdef DEBUG
  GLint len, success;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
  if (!success) {
    auto log = new char[len];
    glGetProgramInfoLog(m_id, len, NULL, log);
    std::cout << log << std::endl;
    delete[] log;
  }
#endif
}

GLProgram::~GLProgram() { glDeleteProgram(m_id); }

void GLProgram::use() { glUseProgram(m_id); }

GLUniform GLProgram::getUniform(const char *location) {
  return GLUniform(*this, location);
}
