#pragma once

#include "gl/buffer.hpp"
#include "gl/gl.hpp"

class GLVertexArray {
  GLuint m_id;

public:
  GLVertexArray();
  GLVertexArray(const GLVertexArray &) = delete;
  GLVertexArray(GLVertexArray &&) = delete;
  ~GLVertexArray();

  void attachIBO(const GLElementArrayBuffer& ibo);
  void attachSSBO(const GLShaderStorageBuffer& ssbo);
  void attachAttrib(const GLVertexArrayBuffer& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset);

  void bind() const;
  void unbind() const;
};
