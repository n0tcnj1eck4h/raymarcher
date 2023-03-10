#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Camera {
  float nearZ, farZ, aspect, fov;
  glm::mat4 view, proj, viewproj;

public:
  Camera();
  glm::vec3 pos, dir;
  void updateProj();
  void updateViewProj();
  const glm::mat4 &getViewProj() const;
};
