cmake_minimum_required(VERSION 3.25)
project(reaktory LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

include(deps/ext.cmake)

add_executable(main src/main.cpp)
target_link_libraries(main
  raylib
  # SDL2::SDL2-static
  imgui
  rlimgui

  xlsxio_write_STATIC
)
