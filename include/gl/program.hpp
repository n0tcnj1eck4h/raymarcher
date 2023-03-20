#pragma once

#include "gl/uniform.hpp"

class GLProgram {
  GLuint m_id;

protected:
  GLUniform getUniform(const char *location);

public:
  GLProgram(const char *vert, const char *frag);
  GLProgram(const GLProgram &) = delete;
  GLProgram(GLProgram &&) = delete;
  ~GLProgram();
  void use();

  friend class GLUniform;
};
