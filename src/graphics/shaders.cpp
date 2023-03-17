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

    out vec2 screenPos;
    
    void main() {
        screenPos = aPos.xy;
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
    in vec2 screenPos;

    const float EPSILON = 0.005;
    const float focal_length = 2.5;
    const vec3 eye = vec3(0.0, 0.0, -focal_length - 1);

    float sphereSDF(vec3 pos) {
        return length(pos) - 1.0;
    }

    float sceneSDF(vec3 pos) {
        return sphereSDF(pos);
    }

    vec3 estimateNormal(vec3 p) {
        return normalize(vec3(
            sceneSDF(vec3(p.x + EPSILON, p.y, p.z)) - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)),
            sceneSDF(vec3(p.x, p.y + EPSILON, p.z)) - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)),
            sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)) - sceneSDF(vec3(p.x, p.y, p.z - EPSILON))
        ));
    }

    vec3 castRay(vec3 eye, vec3 rd) {
        float depth = 0.0;
        for(int i = 0; i < 100; i++) {
            float dist = sceneSDF(eye + rd * depth);

            if(dist < EPSILON) {
                return estimateNormal(eye + rd * depth);
            }

            depth += dist;
            if(depth > 100.0) {
                return vec3(0.0, 0.0, 0.0);
            }
        }

        return vec3(0.0, 0.0, 0.0);
    }

    void main() {

        vec3 rd = normalize(vec3(screenPos.xy, focal_length));

        FragColor = vec4((vec3(1.0,1.0,1.0) + castRay(eye, rd)) / 2.0, 1.0);
        // FragColor = vec4(rd, 1.0);
    }
)glsl";
