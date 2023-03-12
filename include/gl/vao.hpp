#pragma once

#include "gl/buffer.hpp"
#include "gl/gl.hpp"
#include "gl/object.hpp"

class GLVertexArray : public GLObject {
  GLuint m_id;

public:
  GLVertexArray();
  GLVertexArray(const GLVertexArray &) = delete;
  GLVertexArray(GLVertexArray &&) = delete;
  ~GLVertexArray();

  void attachIBO(const GLElementArrayBuffer& ibo);
  void attachAttrib(const GLVertexArrayBuffer& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset);

  void bind() const final;
  void unbind() const final;
};
