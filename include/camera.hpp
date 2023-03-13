#pragma once
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

class Camera {
  float nearZ, farZ, aspect, fov;
  glm::vec3 pos, dir;

public:
  Camera(float nearZ, float farZ, float aspect_ratio, float fov);
  void moveLocal(const glm::vec3 &delta);
  void moveGlobal(const glm::vec3 &delta);
  void rotateY(float angle);
  void rotateX(float angle);

  glm::mat4 getViewProj() const;
};
