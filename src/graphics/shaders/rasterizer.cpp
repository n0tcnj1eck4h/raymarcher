#include "graphics/shaders/rasterizer.hpp"
#include "gl/uniform.hpp"
#include "config.hpp"


static const char *vert_source = GLSL_VERSION_HEADER
    R"glsl(
    layout(location = 0) in vec3 aPos;
    
    uniform mat4 model;
    uniform mat4 viewProj;
    
    void main() {
      gl_Position = viewProj * model * vec4(aPos, 1.0);
    }
)glsl";

static const char *frag_source = GLSL_VERSION_HEADER
    R"glsl(
    out vec4 FragColor;
    uniform vec3 color;
    
    void main() {
      FragColor = vec4(color, 1.0);
    }
)glsl";

RasterizerShader::RasterizerShader()
    : GLProgram(vert_source, frag_source),
      m_viewProjUniform(getUniform("viewProj")),
      m_colorUniform(getUniform("color")), m_modelUniform(getUniform("model")) {
}

void RasterizerShader::setDrawColor(const glm::vec3 &col) {
  use();
  m_colorUniform.vec3(col);
}

void RasterizerShader::setModelMatrix(const glm::mat4 &mat) {
  use();
  m_modelUniform.matrix(mat);
}

void RasterizerShader::setViewProjMatrix(const glm::mat4 &mat) {
  use();
  m_viewProjUniform.matrix(mat);
}
