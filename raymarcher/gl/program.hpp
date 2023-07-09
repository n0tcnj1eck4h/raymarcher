#pragma once

#include "gl/gl.hpp"

class GLProgram {
  GLuint m_id;

public:
  GLuint ID() const { return m_id; }
  GLProgram(const char *vert, const char *frag);
  GLProgram(const GLProgram &) = delete;
  GLProgram(GLProgram &&) = delete;
  ~GLProgram();
  void use() const;
};
