include(deps/CPM.cmake)
set(CPM_USE_LOCAL_PACKAGES ON)

macro (install)
endmacro ()

macro (find_package)
endmacro ()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  message("Ccache enabled")
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM} base_dir=${PROJECT_SOURCE_DIR} hash_dir=false)
endif()

# #math
# message(glm)
# CPMAddPackage(
#   NAME glm
#   VERSION 1.0.3
#   GITHUB_REPOSITORY g-truc/glm
#   GIT_TAG 1.0.3
#   OPTIONS
#     "GLM_ENABLE_CXX_20 ON"
# )

# set(CPM_USE_LOCAL_PACKAGES OFF)
#
# function(original_add_library)
#   _add_library(${ARGV})
# endfunction()
#
# function(add_library target)
#   # if(target IN_LIST UNWANTED_TARGETS)
#   #   message(STATUS "Excluding library target: ${target}")
#   #   return()  # Skip creating the target
#   # endif()
#   if (target STREQUAL "SDL3::SDL3")
#     message("cnsdcnsdkjncdsc>>>>>>> sdl3 : ${ARGV2}")
#   endif()
#   if(target STREQUAL "SDL3::SDL3" AND ARGV2 STREQUAL "SDL3")
#     return()  # Skip creating the target
#   endif()
#   original_add_library(${ARGV})  # Create the target if allowed
# endfunction()
#
# #sdl
# message(sdl3)
# CPMAddPackage(
#   NAME SDL3
#   VERSION 3.2.28
#   GITHUB_REPOSITORY libsdl-org/sdl
#   GIT_TAG "release-3.2.28"
#   OPTIONS
#     "SDL_STATIC ON"
#     "SDL_SHARED OFF"
#     "SDL_EXAMPLES OFF"
#     # "SDL_RENDER_D3D OFF"
# )
# set(CPM_USE_LOCAL_PACKAGES ON)
#
# set(SDL3_FOUND TRUE CACHE BOOL "" FORCE)
# if (NOT TARGET SDL3::SDL3)
#   add_library(SDL3::SDL3 ALIAS SDL3-static)
# endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED CONFIG)
# message(${CMAKE_THREAD_LIBS_INIT})

#rendering
message(raylib)
CPMAddPackage(
  NAME raylib
  GITHUB_REPOSITORY raysan5/raylib
  GIT_TAG 970531d112fd535c13b45442468dded784b9779e
  OPTIONS
    # "PLATFORM SDL"
    "OPENGL_VERSION 3.3"
    "USE_AUDIO OFF"
    "GLFW_BUILD_WAYLAND ON"
    "GLFW_BUILD_X11 ON"
    "BUILD_SHARED_LIBS OFF"
)

file(GLOB imgui_SRC
  deps/imgui/*.hpp
  deps/imgui/*.cpp
)
add_library(
  imgui STATIC ${imgui_SRC}
)
target_include_directories(imgui INTERFACE deps/imgui)
CPMAddPackage(
  NAME rlimgui
  GITHUB_REPOSITORY raylib-extras/rlImGui
  GIT_TAG 286e11acd6c785004c9550c7ed3762add2ae3d47
  DOWNLOAD_ONLY
)
add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_link_libraries(rlimgui PRIVATE imgui raylib)
target_include_directories(rlimgui INTERFACE ${rlimgui_SOURCE_DIR})

CPMAddPackage(
  NAME zlib
  VERSION 1.3.1.2
  GITHUB_REPOSITORY madler/zlib
  GIT_TAG v1.3.1.2
  OPTIONS
    "ZLIB_BUILD_SHARED OFF"
    "ZLIB_BUILD_STATIC ON"
    "ZLIB_BUILD_TESTING OFF"
    "ZLIB_INSTALL OFF"
)

set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
add_library(ZLIB::ZLIB ALIAS zlibstatic)

# #loading models
# message(assimp)
# CPMAddPackage(
#   NAME assimp
#   VERSION 6.0.4
#   GITHUB_REPOSITORY assimp/assimp
#   GIT_TAG v6.0.4
#   OPTIONS
#     "BUILD_SHARED_LIBS OFF"
#     "ASSIMP_BUILD_TESTS OFF"
#     "ASSIMP_INSTALL OFF"
#     "ASSIMP_BUILD_DOCS OFF"
#     "ASSIMP_BUILD_ZLIB ON"
#     # "ZLIB_LIBRARY ${zlib_LIBRARIES}"
#     # "ZLIB_INCLUDE_DIR ${zlib_INCLUDE_DIRS}"
# )

# set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
# add_library(ZLIB::ZLIB UNKNOWN IMPORTED)
# set(ZLIB_LIBRARY "${zlib_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}zlibstatic${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}" CACHE PATH "" FORCE)
# set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR};${zlib_BINARY_DIR};${assimp_SOURCE_DIR}/contrib/unzip" CACHE ARRAY "" FORCE)
# set_target_properties(ZLIB::ZLIB PROPERTIES IMPORTED_LOCATION "${ZLIB_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${zlib_INCLUDE_DIR}")

#excel
set(CPM_USE_LOCAL_PACKAGES OFF)
message(expat)
CPMAddPackage(
  NAME expat
  VERSION 2.7.4
  GITHUB_REPOSITORY libexpat/libexpat
  GIT_TAG R_2_7_4
  DOWNLOAD_ONLY
)
set(CPM_USE_LOCAL_PACKAGES ON)

set(EXPAT_BUILD_TOOLS OFF)
set(EXPAT_SHARED_LIBS OFF)
set(EXPAT_BUILD_EXAMPLES OFF)
set(EXPAT_ENABLE_INSTALL OFF)

set(EXPAT_DIR "EXPAT_DIR ${expat_SOURCE_DIR}/expat")

message(STATUS expatsrc ${expat_SOURCE_DIR})
add_subdirectory(${expat_SOURCE_DIR}/expat ${CMAKE_BINARY_DIR}/expat.dir)

# add_library(ZLIB::ZLIB alias zlib_static)

message(libzip)
CPMAddPackage(
  NAME libzip
  VERSION 1.11.4
  GITHUB_REPOSITORY nih-at/libzip
  GIT_TAG v1.11.4
  OPTIONS
    "ENABLE_OPENSSL OFF"
    "ENABLE_COVERAGE OFF"
    "ENABLE_ZSTD OFF"
    "ENABLE_COMMONCRYPTO OFF"
    "ENABLE_GNUTLS OFF"
    "ENABLE_MBEDTLS OFF"
    "ENABLE_BZIP2 OFF"
    "ENABLE_LZMA OFF"
    "ENABLE_FDOPEN OFF"
    "BUILD_SHARED_LIBS OFF"
    "BUILD_TOOLS OFF"
    "BUILD_DOC OFF"
    "BUILD_EXAMPLES OFF"
    "BUILD_OSSFUZZ OFF"
    "BUILD_REGRESS OFF"
    "LIBZIP_DO_INSTALL OFF"
    "BUILD_SHARED_LIBS OFF"

    "CFLAGS -I${libzip_SOURCE_DIR}"
)

target_include_directories(zip PUBLIC ${libzip_SOURCE_DIR})

get_target_property(LIBZIP_INCLUDES libzip::zip INCLUDE_DIRECTORIES)
get_target_property(EXPAT_INCLUDES expat INCLUDE_DIRECTORIES)
list(APPEND LIBZIP_INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps> $<BUILD_INTERFACE:${libzip_BINARY_DIR}>)

message(xlsxio)
CPMAddPackage(
  NAME xlsxio
  VERSION 0.2.36
  GITHUB_REPOSITORY brechtsanders/xlsxio
  GIT_TAG 0.2.36
  OPTIONS
    "CMAKE_POLICY_VERSION_MINIMUM 3.5"
    "BUILD_STATIC ON"
    "BUILD_SHARED OFF"
    "BUILD_TOOLS OFF"
    "BUILD_DOCUMENTATION OFF"
    "BUILD_PC_FILES OFF"
    "BUILD_EXAMPLES OFF"
    # "WITH_MINIZIP_NG ON"
    "WITH_LIBZIP ON"

    "EXPAT_INCLUDE_DIR ${EXPAT_INCLUDES}"
    "EXPAT_LIBRARIES expat"
    "EXPAT_DIR ${EXPAT_DIR}"
    "ZLIB_LIBRARY "
    "ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRS}"
    # "ZLIB_DIR ${zlib_SOURCE_DIR}"

    "LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDES}"
)

target_link_libraries(xlsxio_read_STATIC zip)
target_include_directories(xlsxio_read_STATIC PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps>)

target_link_libraries(xlsxio_write_STATIC zip)
target_include_directories(xlsxio_write_STATIC PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deps>)
