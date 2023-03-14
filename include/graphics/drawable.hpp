#pragma once

#include "graphics/renderer.hpp"

class Drawable {
public:
  virtual void draw(const Renderer &renderer) = 0;
};
