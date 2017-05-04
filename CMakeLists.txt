cmake_minimum_required(VERSION 3.5)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp RasterUtils.h ClipUtils.h ClipUtils.cpp RasterUtils.cpp Sphere.cpp Sphere.h Rasterizer.cpp Rasterizer.h)
link_libraries(X11)
add_executable(untitled ${SOURCE_FILES})