#pragma once

#include "gl/program.hpp"
#include <glm/fwd.hpp>

class RaymarcherShader : public GLProgram {
  GLUniform m_dirUniform;
  GLUniform m_eyeUniform;
  GLUniform m_aspectRatioUniform;
  GLUniform m_shapeIDUniform;
  GLUniform m_maxStepsUniform;
  GLUniform m_maxDistUniform;
  GLUniform m_epsilonUniform;

public:
  RaymarcherShader();
  void setCameraPosition(const glm::vec3 &pos);
  void setCameraDirection(const glm::vec3 &dir);
  void setAspectRatio(float aspect_ratio);
  void setShapeID(i32 shapeID);
  void setMaxSteps(i32 steps);
  void setMaxDist(float dist);
  void setEpsilon(float epsilon);
};
