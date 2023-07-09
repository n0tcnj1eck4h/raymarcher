cmake_minimum_required(VERSION 3.15)

project(raymarcher-project VERSION 0.1 LANGUAGES CXX)

option(USE_PREHISTORIC_GL "Use OpenGL 3.2 instead of 4.6" OFF)
option(DEBUG "Debug" OFF)

add_executable(raymarcher 
  raymarcher/main.cpp
  raymarcher/gl/program.cpp
  raymarcher/gl/uniform.cpp
  raymarcher/gl/vao.cpp
  raymarcher/gl/buffer.cpp
  raymarcher/game.cpp
  raymarcher/imgui/imgui_demo.cpp
  raymarcher/imgui/imgui_draw.cpp
  raymarcher/imgui/imgui_tables.cpp
  raymarcher/imgui/imgui_impl_opengl3.cpp
  raymarcher/imgui/imgui.cpp
  raymarcher/imgui/imgui_impl_sdl2.cpp
  raymarcher/imgui/imgui_widgets.cpp
  raymarcher/graphics/camera.cpp
  raymarcher/graphics/renderer.cpp
  raymarcher/graphics/shaders/raymarcher.cpp
  raymarcher/graphics/shaders/ddamarcher.cpp
)

if(USE_PREHISTORIC_GL)
  target_compile_definitions(raymarcher PUBLIC USE_PREHISTORIC_GL)
endif()

if(DEBUG)
  target_compile_definitions(raymarcher PUBLIC DEBUG)
endif()

target_include_directories(raymarcher PUBLIC raymarcher)
target_include_directories(raymarcher PUBLIC raymarcher/imgui)
target_compile_features(raymarcher PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
target_include_directories(raymarcher PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(raymarcher PUBLIC ${SDL2_LIBRARIES})

find_package(glbinding REQUIRED)
target_link_libraries(raymarcher PUBLIC glbinding::glbinding glbinding::glbinding-aux)

find_package(glm REQUIRED)
target_include_directories(raymarcher PUBLIC ${GLM_INCLUDE_DIRS})
