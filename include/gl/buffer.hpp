#pragma once
#include "gl/gl.hpp"
#include "gl/object.hpp"
#include "types.hpp"

class GLBuffer : public GLObject {
protected:
  GLenum m_target;

  GLBuffer(GLenum target);
  GLBuffer(const GLBuffer &) = delete;
  GLBuffer(GLBuffer &&) = delete;
  ~GLBuffer();

public:
  void bind() const final;
  void unbind() const final;
  void data(const void *data, GLsizeiptr size);
};

class GLVertexArrayBuffer : public GLBuffer {
public:
  GLVertexArrayBuffer() : GLBuffer(GL_ARRAY_BUFFER){};
};

class GLElementArrayBuffer : public GLBuffer {
public:
  GLElementArrayBuffer() : GLBuffer(GL_ELEMENT_ARRAY_BUFFER){};
};
