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

    out vec4 FragColor;
    in vec3 rayDirection;
    uniform vec3 eye;
    vec3 ray = eye;

    const float EPSILON = 1.0 / 4096.0 / 2.0;

    vec3 castRay(vec3 rd) {

        vec3 rayUnitStepSize = vec3(
            sqrt(1 + pow(rd.y / rd.x, 2) + pow(rd.z / rd.x, 2)),
            sqrt(1 + pow(rd.x / rd.y, 2) + pow(rd.z / rd.y, 2)),
            sqrt(1 + pow(rd.y / rd.z, 2) + pow(rd.x / rd.z, 2))
        );

        ivec3 step = ivec3(sign(rd));
        ivec3 ipos = ivec3(ceil(eye));

        vec3 rayLength1D = vec3(
            float(rd.x >= 0.0) - fract(ray.x) / rd.x,
            float(rd.y >= 0.0) - fract(ray.y) / rd.y,
            float(rd.z >= 0.0) - fract(ray.z) / rd.z
        );

        for(int i = 0; i < 64; i++) {
            float min_dist = min(min(rayLength1D.x, rayLength1D.y), rayLength1D.z);
            ivec3 min_mask = ivec3(rayLength1D.x <= min_dist, rayLength1D.y <= min_dist, rayLength1D.z <= min_dist);

            ipos += step * min_mask;
            rayLength1D += rayUnitStepSize * min_mask;
            if(length(ipos) > 16) break;
        }

        return mod(vec3(ipos) / 16.0, 1);
    }

    void main() {
        vec3 rd = normalize(rayDirection);
        FragColor = vec4(castRay(rd), 1.0);
    }
)glsl";

DDAMarcherShader::DDAMarcherShader()
    : GLProgram(vert_source, frag_source), directionUniform(*this, "dir"),
      eyeposUniform(*this, "eye") {}
