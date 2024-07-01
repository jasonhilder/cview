cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Source files directory
set(SRC_DIR src)

# Object files directory
set(BUILD_DIR build)
file(MAKE_DIRECTORY ${BUILD_DIR})

# List of source files
file(GLOB SRCS "${SRC_DIR}/*.c")

# Output executable
set(TARGET main)

# Find or fetch libgit2
include(FetchContent)
FetchContent_Declare(
  libgit2
  GIT_REPOSITORY https://github.com/libgit2/libgit2.git
  GIT_TAG v1.3.0  # Specify the version you want
)
FetchContent_MakeAvailable(libgit2)

# Add executable target
add_executable(${TARGET} ${SRCS})

# Link libraries
target_link_libraries(${TARGET} PRIVATE git2)

# Include libgit2 headers
target_include_directories(${TARGET} PRIVATE ${libgit2_SOURCE_DIR}/include)

