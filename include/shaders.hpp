#pragma once

static const char *vertex_shader_source = R"glsl(
  #version 330 core

  layout(location = 0) in vec3 aPos;

  uniform mat4 model;
  uniform mat4 viewProj;

  void main() {
    gl_Position = viewProj * model * vec4(aPos, 1.0);
  }
)glsl";

static const char *fragment_shader_source = R"glsl(
  #version 330 core

  out vec4 FragColor;
  uniform vec3 color;

  void main() {
    FragColor = vec4(color, 1.0);
  }
)glsl";
