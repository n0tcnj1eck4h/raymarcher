#include "camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera()
    : pos(2.0, 2.0, 2.0), dir(-1.0, -1.0, -1.0), nearZ(0.1), farZ(50.0),
      aspect(16.0 / 9.0), fov(80.0) {
  updateProj();
  updateViewProj();
}

void Camera::updateViewProj() {
  view = glm::lookAt(pos, pos + dir, glm::vec3(0.0, -1.0, 0.0));
  viewproj = proj * view;
}

void Camera::updateProj() { proj = glm::perspective(fov, aspect, nearZ, farZ); }

const glm::mat4 &Camera::getViewProj() const { return viewproj; }
