#include "renderer.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"
#include "shaders.hpp"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const glm::vec3 box_vertices[] = {
    {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
};

const GLuint solid_box_indices[] = {0, 2, 1, 2, 3, 1, 0, 4, 6, 6, 2, 0,
                                    6, 7, 3, 3, 2, 6, 5, 1, 3, 3, 7, 5,
                                    5, 4, 0, 0, 1, 5, 5, 7, 6, 6, 4, 5};

const GLuint line_box_indices[] = {0, 1, 0, 2, 3, 1, 3, 2, 0, 4, 1, 5,
                                   2, 6, 3, 7, 4, 5, 4, 6, 7, 5, 7, 6};

extern "C" void messageCallback(GLenum, GLenum type, GLuint, GLenum, GLsizei,
                                const GLchar *message, const void *) {
  std::cout << "OpenGL Callback: "
            << (type == GL_DEBUG_TYPE_ERROR ? "ERROR " : "Message ") << message
            << std::endl;
}

GLuint createShader(const char *source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

#ifdef DEBUG
  GLint len, success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (!success) {
    auto log = new char[len];
    glGetShaderInfoLog(shader, len, NULL, log);
    std::cout << log << std::endl;
    delete[] log;
  }
#endif

  return shader;
}

GLuint createProgram(GLuint vert_shader, GLuint frag_shader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);

#ifdef DEBUG
  GLint len, success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
  if (!success) {
    auto log = new char[len];
    glGetProgramInfoLog(program, len, NULL, log);
    std::cout << log << std::endl;
    delete[] log;
  }
#endif

  return program;
}

GLuint createBuffer(GLsizeiptr size, const void *data) {
  GLuint buf;
  glCreateBuffers(1, &buf);
  glNamedBufferStorage(buf, size, data, GL_CLIENT_STORAGE_BIT);
  return buf;
}

GLuint createVertexArray(GLuint vbo, GLuint ibo) {
  GLuint vao;
  glCreateVertexArrays(1, &vao);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(glm::vec3));
  glVertexArrayElementBuffer(vao, ibo);
  return vao;
}

GLuint Renderer::program;
GLuint Renderer::vbo, Renderer::ibo, Renderer::solid_box_vao;
GLuint Renderer::viewproj_uniform;
GLuint Renderer::model_uniform;
GLuint Renderer::color_uniform;

void Renderer::init() {
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.5f);
  glViewport(0, 0, 1080, 720);
  glClearColor(0, 0, 1.0, 1.0);
  glDebugMessageCallback(messageCallback, nullptr);

  GLuint vert_shader = createShader(vertex_shader_source, GL_VERTEX_SHADER);
  GLuint frag_shader = createShader(fragment_shader_source, GL_FRAGMENT_SHADER);
  program = createProgram(vert_shader, frag_shader);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  glUseProgram(program);

  vbo = createBuffer(sizeof(box_vertices), box_vertices);
  ibo = createBuffer(sizeof(solid_box_indices), solid_box_indices);
  solid_box_vao = createVertexArray(vbo, ibo);

  viewproj_uniform = glGetUniformLocation(program, "viewProj");
  model_uniform = glGetUniformLocation(program, "model");
  color_uniform = glGetUniformLocation(program, "color");
  glProgramUniform3fv(program, color_uniform, 1,
                      glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
}

void Renderer::deinit() {
  glDeleteVertexArrays(1, &solid_box_vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteProgram(program);
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::draw() {}

void Renderer::updateView(const Camera &camera) {
  const auto &m = camera.getViewProj();
  glProgramUniformMatrix4fv(program, viewproj_uniform, 1, GL_FALSE,
                            glm::value_ptr(m));
}

void Renderer::drawCube(const glm::mat4 &transform) {
  glBindVertexArray(solid_box_vao);
  glProgramUniformMatrix4fv(program, model_uniform, 1, GL_FALSE,
                            glm::value_ptr(transform));
  glDrawElements(GL_TRIANGLES,
                 sizeof(solid_box_indices) / sizeof(*solid_box_indices),
                 GL_UNSIGNED_INT, 0);
}
