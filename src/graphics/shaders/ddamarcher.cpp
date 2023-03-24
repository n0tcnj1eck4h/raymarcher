#include "graphics/shaders/ddamarcher.hpp"
#include "config.hpp"

static const char *vert_source = GLSL_VERSION_HEADER
    R"glsl(
    layout(location = 0) in vec2 aPos;

    out vec3 rayDirection;
    uniform vec3 dir;
    
    const float focal_length = 1.5;
    const float aspect_ratio = 16.0 / 9.0;
    const vec3 up = vec3(0, 1, 0);

    void main() {
        const vec3 right = normalize(cross(dir, up));
        const vec3 up_local = normalize(cross(right, dir));

        rayDirection = normalize(aPos.x * right * aspect_ratio + aPos.y * up_local + focal_length * dir);
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

static const char *frag_source = GLSL_VERSION_HEADER
    R"glsl(
    layout(std430, binding = 2) buffer bData {
        float r;
        float g;
        float b;
    };

    out vec4 FragColor;
    in vec3 rayDirection;
    uniform vec3 eye;
    vec3 ray = eye;

    void main() {
        vec3 rd = normalize(rayDirection);
        FragColor = vec4(r, g, b, 1.0);
    }
)glsl";

DDAMarcherShader::DDAMarcherShader()
    : GLProgram(vert_source, frag_source),
      m_dirUniform(getUniform("dir")), m_eyeUniform(getUniform("eye")) {}

void DDAMarcherShader::setCameraDirection(const glm::vec3 &dir) {
  use();
  m_dirUniform.vec3(dir);
}

void DDAMarcherShader::setCameraPosition(const glm::vec3 &pos) {
  use();
  m_eyeUniform.vec3(pos);
}
