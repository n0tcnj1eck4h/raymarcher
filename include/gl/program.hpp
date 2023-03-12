#pragma once

#include "gl/gl.hpp"

class GLProgram {

public:
  GLuint m_id;
  GLProgram(const char *vert, const char *frag);
  ~GLProgram();
  void use();
};
