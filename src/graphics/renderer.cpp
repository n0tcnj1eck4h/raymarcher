#include "graphics/renderer.hpp"
#include "config.hpp"
#include "glm/fwd.hpp"
#include "graphics/camera.hpp"
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glm/geometric.hpp>
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

static const glm::vec2 screen_vertices[] = {
    {-1.0f, -1.0f},
    {-1.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, -1.0f},
};

static const GLuint screen_indices[] = {2, 1, 0, 0, 3, 2};

static const GLfloat ssbo_data[] = {0.0f, 0.5f, 1.0f};

#ifndef USE_PREHISTORIC_GL
extern "C" void messageCallback(GLenum, GLenum type, GLuint, GLenum, GLsizei,
                                const GLchar *message, const void *) {
  std::cout << "OpenGL Callback: "
            << (type == GL_DEBUG_TYPE_ERROR ? "ERROR " : "Message ") << message
            << std::endl;
}
#endif

Renderer::Renderer() {

#ifndef USE_PREHISTORIC_GL
  glDebugMessageCallback(messageCallback, nullptr);
  glEnable(GL_DEBUG_OUTPUT);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.5f);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glClearColor(0, 0, 0, 1.0);

  m_vbo.data(box_vertices, sizeof(box_vertices));
  m_ibo.data(solid_box_indices, sizeof(solid_box_indices));
  m_vao.attachAttrib(m_vbo, 0, 3, GL_FLOAT, 0, nullptr);
  m_vao.attachIBO(m_ibo);
  m_vao.unbind();

  m_ibo2.data(screen_indices, sizeof(screen_indices));
  m_vbo2.data(screen_vertices, sizeof(screen_vertices));

  m_vao2.attachAttrib(m_vbo2, 0, 2, GL_FLOAT, 0, nullptr);
  m_vao2.attachIBO(m_ibo2);
  m_vao2.unbind();

  m_rasterizer.setDrawColor(glm::vec3(1, 0, 0));
  m_raymarcher.setCameraDirection(glm::normalize(glm::vec3(0, 10, 3)));
}

void Renderer::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer::draw() {
  m_raymarcher.use();
  m_vao2.bind();
  glDrawElements(GL_TRIANGLES, sizeof(screen_indices) / sizeof(*screen_indices),
                 GL_UNSIGNED_INT, 0);
}

// void Renderer::drawCube(const glm::mat4 &transform) {
//   m_rasterizer.use();
//   m_vao.bind();
//   m_rasterizer.setModelMatrix(transform);
//   glDrawElements(GL_TRIANGLES,
//                  sizeof(solid_box_indices) / sizeof(*solid_box_indices),
//                  GL_UNSIGNED_INT, 0);
// }
