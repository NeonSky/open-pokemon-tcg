cmake_minimum_required(VERSION 3.17)

# "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
SET(CMAKE_TOOLCHAIN_FILE "/usr/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(OpenPokemonTCG VERSION 1.0.0)

# set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE "GLVND")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(GLEW 2.1.0 CONFIG REQUIRED)
find_package(glfw3 3.3.2 CONFIG REQUIRED)
find_package(glm 0.9.9.8 CONFIG REQUIRED)
find_package(Boost 1.72.0 REQUIRED COMPONENTS filesystem)

file(GLOB_RECURSE cpp_files "src/*.cpp")
add_executable(OpenPokemonTCG ${cpp_files})
target_compile_options(OpenPokemonTCG PRIVATE -Werror -Wall -Wextra)

target_link_libraries(OpenPokemonTCG PRIVATE GLEW::GLEW)
target_link_libraries(OpenPokemonTCG PRIVATE glfw)
target_link_libraries(OpenPokemonTCG PRIVATE glm)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(OpenPokemonTCG PRIVATE ${Boost_LIBRARIES})
endif()
