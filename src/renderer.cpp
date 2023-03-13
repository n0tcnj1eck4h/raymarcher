#include "renderer.hpp"
#include "camera.hpp"
#include "config.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shaders.hpp"
#include <iostream>

static const glm::vec3 box_vertices[] = {
    {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
};

static const GLuint solid_box_indices[] = {1, 2, 0, 1, 3, 2, 6, 4, 0, 0, 2, 6,
                                           3, 7, 6, 6, 2, 3, 3, 1, 5, 5, 7, 3,
                                           0, 4, 5, 5, 1, 0, 6, 7, 5, 5, 4, 6};

static const GLuint line_box_indices[] = {0, 1, 0, 2, 3, 1, 3, 2, 0, 4, 1, 5,
                                          2, 6, 3, 7, 4, 5, 4, 6, 7, 5, 7, 6};

#ifndef USE_PREHISTORIC_GL
extern "C" void messageCallback(GLenum, GLenum type, GLuint, GLenum, GLsizei,
                                const GLchar *message, const void *) {
  std::cout << "OpenGL Callback: "
            << (type == GL_DEBUG_TYPE_ERROR ? "ERROR " : "Message ") << message
            << std::endl;
}
#endif

Renderer::Renderer()
    : m_program(vertex_shader_source, fragment_shader_source),
      m_viewprojUniform(m_program.getUniform("viewProj")),
      m_modelUniform(m_program.getUniform("model")),
      m_colorUniform(m_program.getUniform("color")) {
#ifndef USE_PREHISTORIC_GL
  glDebugMessageCallback(messageCallback, nullptr);
  glEnable(GL_DEBUG_OUTPUT);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.5f);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glClearColor(0, 0, 1.0, 1.0);

  m_program.use();

  m_ibo.data(solid_box_indices, sizeof(solid_box_indices));
  m_vbo.data(box_vertices, sizeof(box_vertices));
  m_vao.attachIBO(m_ibo);
  m_vao.attachAttrib(m_vbo, 0, 3, GL_FLOAT, 0, nullptr);

  m_colorUniform.vec3(glm::vec3(1.0, 1.0, 1.0));
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::draw() {}

void Renderer::updateView(const Camera &camera) {
  const auto &m = camera.getViewProj();
  m_viewprojUniform.matrix(m);
}

void Renderer::drawCube(const glm::mat4 &transform) {
  m_vao.bind();
  m_modelUniform.matrix(transform);
  glDrawElements(GL_TRIANGLES,
                 sizeof(solid_box_indices) / sizeof(*solid_box_indices),
                 GL_UNSIGNED_INT, 0);
}
