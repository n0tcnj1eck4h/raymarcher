#include "gl/vao.hpp"
#include <glbinding/gl32core/functions.h>

GLVertexArray::GLVertexArray() {
  glGenVertexArrays(1, &m_id);
}

GLVertexArray::~GLVertexArray() {
  glDeleteVertexArrays(1, &m_id);
}

void GLVertexArray::bind() const { glBindVertexArray(m_id); }

void GLVertexArray::unbind() const { glBindVertexArray(0); }

void GLVertexArray::attachAttrib(const GLVertexArrayBuffer& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset) {
  bind();
  vbo.bind();
  glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(index);
}

void GLVertexArray::attachIBO(const GLElementArrayBuffer& ibo) {
  bind();
  ibo.bind();
}


