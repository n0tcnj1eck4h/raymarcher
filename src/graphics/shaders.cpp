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

    out vec3 rayDirection;
    uniform vec3 dir;
    
    const float focal_length = 2.0;
    const vec3 up = vec3(0, 1, 0);

    void main() {
        const vec3 right = normalize(cross(dir, up));
        const vec3 up_local = normalize(cross(right, dir));

        rayDirection = normalize(aPos.x * right + aPos.y * up_local + focal_length * dir);
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
    in vec3 rayDirection;
    uniform vec3 eye;

    const float EPSILON = 0.0005;

    float sphereSDF(vec3 pos) {
        return length(pos) - 1.0;
    }

    float boxSDF(vec3 p, vec3 b) {
      vec3 q = abs(p) - b;
      return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    }

    float sceneSDF(vec3 pos) {
        return min(min(boxSDF(pos, vec3(0.1, 0.1, 2)), boxSDF(pos, vec3(2, 0.1, 0.1))), boxSDF(pos, vec3(0.1, 2, 0.1)));
        // return max(sphereSDF(pos), -boxSDF(pos, vec3(0.5, 2.0, 0.5)));
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
        float shortestDist = 128.0f;
        ray = mod(ray + vec3(2), 4) - vec3(2);

        for(int i = 0; i < 256; i++) {
            float dist = sceneSDF(ray);
            shortestDist = min(shortestDist, dist);

            if(dist < EPSILON) {
                return (vec3(1) + estimateNormal(ray) * vec3(100-i) / 100) / 2.0;
            }

            ray += dist * rd;
            ray = mod(ray + vec3(2), 4) - vec3(2);

            if(length(ray) > 100.0) {
                return vec3(0.0, 0.0, 0.0);
            }
        }

        return vec3(0.0, 0.0, 0.0);
    }

    void main() {
        vec3 rd = normalize(rayDirection);
        FragColor = vec4(castRay(eye, rd), 1.0);
    }
)glsl";
