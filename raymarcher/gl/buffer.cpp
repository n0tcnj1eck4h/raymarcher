#include <gl/buffer.hpp>

GLBuffer::GLBuffer(GLenum target) : m_target(target) { glGenBuffers(1, &m_id); }

GLBuffer::~GLBuffer() { glDeleteBuffers(1, &m_id); }

void GLBuffer::bind() const { glBindBuffer(m_target, m_id); }

void GLBuffer::unbind() const { glBindBuffer(m_target, 0); }

void GLBuffer::data(const void *data, GLsizeiptr size) {
  bind();
  glBufferData(m_target, size, data, GL_STATIC_READ);
}

#ifndef USE_PREHISTORIC_GL
void GLShaderStorageBuffer::bindBase(GLuint binding_location) {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_location, m_id);
}
#endif
