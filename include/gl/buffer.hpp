#pragma once
#include "gl/gl.hpp"
#include "types.hpp"

class GLBuffer {
protected:
  GLenum m_target;
  GLuint m_id;

  GLBuffer(GLenum target);
  GLBuffer(const GLBuffer &) = delete;
  GLBuffer(GLBuffer &&) = delete;
  ~GLBuffer();

public:
  void bind() const;
  void unbind() const;
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

#ifndef USE_PREHISTORIC_GL
class GLShaderStorageBuffer : public GLBuffer {
public:
  GLShaderStorageBuffer() : GLBuffer(GL_SHADER_STORAGE_BUFFER){};
  void bindBase(GLuint binding_location);
};
#endif
