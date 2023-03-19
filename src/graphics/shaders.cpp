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
    const float focal_length = 1.0;

    float sphereSDF(vec3 pos) {
        return length(pos) - 1.0;
    }

    float boxSDF(vec3 p, vec3 b) {
      vec3 q = abs(p) - b;
      return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
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
        vec3 ray = eye;
        for(int i = 0; i < 100; i++) {
            float dist = sceneSDF(ray);

            if(dist < EPSILON) {
                return estimateNormal(ray);
            }

            ray += dist * rd;

            if(length(ray) > 100.0) {
                return vec3(0.0, 0.0, 0.0);
            }
        }

        return vec3(0.0, 0.0, 0.0);
    }

    void main() {
        const vec3 eye = vec3(2.0, 2.0, -2.0);
        const vec3 dir = normalize(vec3(-2.0, -2.0, 2.0));
        const vec3 up = vec3(0, 1, 0);

        const vec3 right = normalize(cross(dir, up));
        const vec3 up_local = normalize(cross(right, dir));

        vec3 rd = normalize(screenPos.x * right + screenPos.y * up_local + focal_length * dir);

        FragColor = vec4((vec3(1.0,1.0,1.0) + castRay(eye, rd)) / 2.0, 1.0);
        // FragColor = vec4(rd, 1.0);
    }
)glsl";
