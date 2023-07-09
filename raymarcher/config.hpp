#pragma once

#define WINDOW_WIDTH (1080)
#define WINDOW_HEIGHT (WINDOW_WIDTH * 9 / 16)

// Use CPPFLAGS while building
// #define USE_PREHISTORIC_GL

#ifdef USE_PREHISTORIC_GL
#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 2
#define GLSL_VERSION_HEADER "#version 330 core"
#else
#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 6
#define GLSL_VERSION_HEADER "#version 460 core"
#endif
