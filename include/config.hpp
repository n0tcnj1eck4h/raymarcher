#pragma once

#define WINDOW_WIDTH (1080 * 1.5)
#define WINDOW_HEIGHT (720 * 1.5)

// Use CPPFLAGS while building
// #define USE_PREHISTORIC_GL

#ifdef USE_PREHISTORIC_GL
#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 2
#else
#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 6
#endif
