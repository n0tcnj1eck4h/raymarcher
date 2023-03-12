#pragma once

#include "gl/gl.hpp"

class GLObject {
protected:
  GLuint m_id;

public:
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
};
