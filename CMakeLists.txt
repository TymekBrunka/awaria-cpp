cmake_minimum_required(VERSION 3.25)
project(reaktory LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

include(deps/ext.cmake)

add_library(assets STATIC src/static.cpp)
target_include_directories(assets PRIVATE src)

add_library(ui STATIC
  src/ui/Style.hpp
  src/ui/Components.hpp

  src/ui/CompGlobals.cpp
  src/ui/Style.cpp

  src/ui/Menu.cpp
  src/ui/SidePanel.cpp
  src/ui/TableRow.cpp
  src/ui/MainPanel.cpp
)
target_include_directories(ui PUBLIC src src/ui)
target_link_libraries(ui PUBLIC imgui rlimgui raylib)

add_executable(main src/main.cpp)
target_link_libraries(main
  PRIVATE
  raylib
  # SDL2::SDL2-static
  # imgui
  rlimgui
  assets
  ui

  xlsxio_write_STATIC
)
