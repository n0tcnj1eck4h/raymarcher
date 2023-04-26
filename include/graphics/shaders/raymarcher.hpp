#pragma once

#include "gl/program.hpp"
#include "gl/uniform.hpp"
#include <glm/fwd.hpp>

class RaymarcherShader : public GLProgram {
public:
  GLUniform<glm::vec3> directionUniform;
  GLUniform<glm::vec3> eyeposUniform;
  GLUniform<float> aspectRatioUniform;
  GLUniform<int> shapeIDUniform;
  GLUniform<int> maxStepsUniform;
  GLUniform<float> maxDistanceUniform;
  GLUniform<float> epsilonUniform;
  RaymarcherShader();
};
