#include "camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera(float nearZ, float farZ, float aspect_ratio, float fov)
    : nearZ(nearZ), farZ(farZ), aspect(aspect_ratio), fov(fov),
      pos(2.0, 2.0, 2.0), dir(-1.0, -1.0, -1.0) {
  dir = glm::normalize(dir);
}

void Camera::moveLocal(const glm::vec3 &delta) {
  // Coś tu jest powalone z tym iloczynem
  glm::vec3 right = glm::cross(dir, glm::vec3(0, 1, 0));
  right = glm::normalize(right);
  pos += right * delta.x;
  pos += dir * delta.z;
  pos.y += delta.y;
}
void Camera::moveGlobal(const glm::vec3 &delta) { pos += delta; }

void Camera::rotateY(float angle) { dir = glm::rotateY(dir, angle); }
void Camera::rotateX(float angle) {
  glm::vec3 right = glm::cross(dir, glm::vec3(0, -1, 0));
  right = glm::normalize(right);
  dir = glm::rotate(dir, angle, right);
}

glm::mat4 Camera::getViewProj() const {
  glm::mat4 proj = glm::perspective(fov, aspect, nearZ, farZ);
  glm::mat4 view = glm::lookAt(pos, pos + dir, glm::vec3(0.0, -1.0, 0.0));
  glm::mat4 viewproj = proj * view;
  return viewproj;
}
