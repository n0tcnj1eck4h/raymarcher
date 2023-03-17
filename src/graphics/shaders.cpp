#include "config.hpp"

#ifdef USE_PREHISTORIC_GL
#define GLSL_VERSION_HEADER "#version 330 core"
#else
#define GLSL_VERSION_HEADER "#version 460 core"
#endif

const char *vertex_shader_source = GLSL_VERSION_HEADER
    R"glsl(
    layout(location = 0) in vec3 aPos;
    
    uniform mat4 model;
    uniform mat4 viewProj;
    
    void main() {
      gl_Position = viewProj * model * vec4(aPos, 1.0);
    }
)glsl";

const char *fragment_shader_source = GLSL_VERSION_HEADER
    R"glsl(
    out vec4 FragColor;
    uniform vec3 color;
    
    void main() {
      FragColor = vec4(color, 1.0);
    }
)glsl";

const char *vertex_shader_source2 = GLSL_VERSION_HEADER
    R"glsl(
    layout(location = 0) in vec2 aPos;
    
    void main() {
      gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

const char *fragment_shader_source2 = GLSL_VERSION_HEADER
    R"glsl(
    layout(std430, binding = 2) buffer bData {
        float r;
        float g;
        float b;
    };

    out vec4 FragColor;
    
    void main() {
      FragColor = vec4(r, g, b, 1.0);
    }
)glsl";
