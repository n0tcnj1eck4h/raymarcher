#include "graphics/shaders/raymarcher.hpp"
#include "config.hpp"

static const char *vert_source = GLSL_VERSION_HEADER
    R"glsl(
    layout(location = 0) in vec2 aPos;

    out vec3 rayDirection;
    uniform vec3 dir;
    
    const float focal_length = 1.5;
    uniform float aspect_ratio;
    const vec3 up = vec3(0, 1, 0);

    void main() {
        vec3 right = normalize(cross(dir, up));
        vec3 up_local = normalize(cross(right, dir));

        rayDirection = normalize(aPos.x * right * aspect_ratio + aPos.y * up_local + focal_length * dir);
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)glsl";

static const char *frag_source = GLSL_VERSION_HEADER
    R"glsl(
    // layout(std430, binding = 2) buffer bData {
    //     float r;
    //     float g;
    //     float b;
    // };

    uniform vec3 eye;
    uniform int shapeID;
    uniform int steps = 128;
    uniform float max_dist = 2048.0;
    uniform float EPSILON = 0.025;

    out vec4 FragColor;
    in vec3 rayDirection;

    float sphereSDF(vec3 ray, vec3 pos) {
        return length(ray - pos) - 1.0;
    }

    float boxSDF(vec3 ray, vec3 b) {
      vec3 q = abs(ray) - b;
      return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    }

    float sceneSDF(vec3 ray) {
        ray = mod(ray + vec3(4), 8) - vec3(4);
        if(shapeID == 0)
            return sphereSDF(ray, vec3(0, 0, 0));
        else if(shapeID == 1)
            return boxSDF(ray, vec3(1, 1, 1));
        else if(shapeID == 2) {
            float floor    = min(boxSDF(ray, vec3(1.5, 0.1, 2.0)), boxSDF(ray, vec3(2.0, 0.1, 1.5)));
            float walls    = min(boxSDF(ray, vec3(0.2, 2.0, 1.0)), boxSDF(ray, vec3(1.0, 2.0, 0.2)));
            return min(floor, walls);
        } else {
            float shape1 = sphereSDF(ray, vec3(-0.5, 0, 0));
            float shape2 = sphereSDF(ray, vec3(0.5, 0, 0));
            float shaft = boxSDF(ray - vec3(0, 2, 0), vec3(0.5, 2, 0.5));
            return min(min(shape1, shape2), shaft);
        }
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
        float ray_length = 0.0;

        for(int i = 0; i < steps; i++) {
            float dist = sceneSDF(ray);

            if(dist < EPSILON) {
                vec3 normal = estimateNormal(ray);
                float distance_norm = 1.0 - ray_length / max_dist;
                float ambient = 1.0 - float(i) / float(steps);
                vec3 color = (vec3(0.5) + normal / 2.0);

                // return color * dot(-rd, normal) * ambient / ray_length;
                return vec3(1) * ambient * color * distance_norm;
            }

            ray_length += dist;
            ray += dist * rd;

            if(ray_length > max_dist) {
                return vec3(0);
            }
        }

        return vec3(0);
    }

    void main() {
        vec3 rd = normalize(rayDirection);
        FragColor = vec4(castRay(eye, rd), 1.0);
    }
)glsl";

RaymarcherShader::RaymarcherShader()
    : GLProgram(vert_source, frag_source), directionUniform(*this, "dir"),
      eyeposUniform(*this, "eye"), aspectRatioUniform(*this, "aspect_ratio"),
      shapeIDUniform(*this, "shapeID"), maxStepsUniform(*this, "steps"),
      maxDistanceUniform(*this, "max_dist"), epsilonUniform(*this, "EPSILON") {}
