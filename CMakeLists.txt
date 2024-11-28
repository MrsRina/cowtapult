cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(COWTAPULT_VERSION 0.1.0)

if(
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
    OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
  )
  set(
    COMPILER_STATIC_INTERNAL_LIBRARIES
    -static-libgcc
    -static-libstdc++
  )
endif()

if(LINUX)
  include_directories(/usr/include/freetype2)

  set(
    COWTAPULT_LINKED_LIBRARIES
    GL
    GLEW
    SDL2main
    SDL2
    freetype
    ekg
    bicudo
  )
endif()

set(EXECUTABLE_OUTPUT_PATH "../bin/")
file(GLOB_RECURSE COWTAPULT_SRC_FILES "./src/*.cpp")

project(
  cowtapult
  VERSION ${COWTAPULT_VERSION}
  DESCRIPTION "a game physics-based for cows"
)

include_directories(./src)

add_executable(
  cowtapult
  ${COWTAPULT_SRC_FILES}
)

target_link_libraries(
  cowtapult
  ${COWTAPULT_LINKED_LIBRARIES}
  ${COMPILER_STATIC_INTERNAL_LIBRARIES}
)