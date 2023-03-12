#pragma once

#include "config.hpp"

#ifdef USE_PREHISTORIC_GL
#include "glbinding/gl32core/gl.h"
using namespace gl32core;
#else
#include "glbinding/gl46core/gl.h"
using namespace gl46core;
#endif
