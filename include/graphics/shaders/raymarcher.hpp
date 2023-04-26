#pragma once

#include "gl/program.hpp"
#include <glm/fwd.hpp>

class RaymarcherShader : public GLProgram {
  GLUniform m_dirUniform;
  GLUniform m_eyeUniform;
  GLUniform m_aspectRatioUniform;
  GLUniform m_shapeIDUniform;

public:
  RaymarcherShader();
  void setCameraPosition(const glm::vec3 &pos);
  void setCameraDirection(const glm::vec3 &dir);
  void setAspectRatio(float aspect_ratio);
  void setShapeID(i32 shapeID);
};
