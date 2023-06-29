cmake_minimum_required(VERSION 3.15)

project(raymarcher-project VERSION 0.1 LANGUAGES CXX)

option(USE_PREHISTORIC_GL "Use OpenGL 3.2 instead of 4.6" OFF)
option(DEBUG "Debug" OFF)

add_executable(raymarcher 
  src/main.cpp
  src/gl/program.cpp
  src/gl/uniform.cpp
  src/gl/vao.cpp
  src/gl/buffer.cpp
  src/game.cpp
  src/imgui/imgui_demo.cpp
  src/imgui/imgui_draw.cpp
  src/imgui/imgui_tables.cpp
  src/imgui/imgui_impl_opengl3.cpp
  src/imgui/imgui.cpp
  src/imgui/imgui_impl_sdl2.cpp
  src/imgui/imgui_widgets.cpp
  src/graphics/camera.cpp
  src/graphics/renderer.cpp
  src/graphics/hwoctree.cpp
  src/graphics/shaders/raymarcher.cpp
  src/graphics/shaders/ddamarcher.cpp
)

if(USE_PREHISTORIC_GL)
  target_compile_definitions(raymarcher PUBLIC USE_PREHISTORIC_GL)
endif()

if(DEBUG)
  target_compile_definitions(raymarcher PUBLIC DEBUG)
endif()

target_include_directories(raymarcher PUBLIC include)
target_include_directories(raymarcher PUBLIC include/imgui)
target_compile_features(raymarcher PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
target_include_directories(raymarcher PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(raymarcher PUBLIC ${SDL2_LIBRARIES})

find_package(glbinding REQUIRED)
target_link_libraries(raymarcher PUBLIC glbinding::glbinding glbinding::glbinding-aux)

find_package(glm REQUIRED)
target_include_directories(raymarcher PUBLIC ${GLM_INCLUDE_DIRS})
