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
    vec3 ray = vec3(0);

    vec3 castRay(vec3 rd) {
        float Sx = sqrt(1 + pow(rd.y / rd.x, 2) + pow(rd.z / rd.x, 2));
        float Sy = sqrt(1 + pow(rd.x / rd.y, 2) + pow(rd.z / rd.y, 2));
        float Sz = sqrt(1 + pow(rd.y / rd.z, 2) + pow(rd.x / rd.z, 2));
        vec3 color = vec3(1);

        ivec3 step = ivec3(sign(rd));
        ivec3 ipos = ivec3(0);

        for(int i = 0; i < 16; i++) {
            vec3 rayabs = mod(abs(ray), 1);
            float dist_x = Sx * (1.0 - rayabs.x);
            float dist_y = Sy * (1.0 - rayabs.y);
            float dist_z = Sz * (1.0 - rayabs.z);

            if(dist_x < dist_y && dist_x < dist_z) {
                ipos += step * ivec3(1, 0, 0);
                ray += dist_x * rd + 0.0001 * step;
            }
            else if(dist_y < dist_x && dist_y < dist_z) {
                ipos += step * ivec3(0, 1, 0);
                ray += dist_y * rd + 0.0001 * step;
            }
            else if(dist_z < dist_y && dist_z < dist_x) {
                ipos += step * ivec3(1, 0, 1);
                ray += dist_z * rd + 0.0001 * step;
            }


        }

        return vec3(abs(ipos)) / length(ray);
    }

    void main() {
        vec3 rd = normalize(rayDirection);
        FragColor = vec4(castRay(rd), 1.0);
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
