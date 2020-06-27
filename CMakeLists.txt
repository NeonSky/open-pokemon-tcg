cmake_minimum_required(VERSION 3.17)

SET(CMAKE_TOOLCHAIN_FILE "/usr/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(OpenPokemonTCG VERSION 1.0.0)

# Global options.
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE "GLVND")

# Global debug options.
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")

# Load configuration cmake files from dependencies.
find_package(glfw3 3.3.2 CONFIG REQUIRED)
find_package(glm 0.9.9.8 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog 1.4.2 CONFIG REQUIRED)
find_package(nlohmann_json 3.7.3 CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.72.0 REQUIRED COMPONENTS filesystem)

# Include local dependencies.
add_subdirectory("vendor")

# Configure executable.
file(GLOB_RECURSE src_files "src/*.c" "src/*.h" "src/*.cpp" "src/*.hpp")
add_executable(OpenPokemonTCG "${src_files}")
target_compile_options(OpenPokemonTCG PRIVATE -Werror -Wall -Wextra)
file(COPY "res" DESTINATION ".")

# Link dependencies.
target_link_libraries(OpenPokemonTCG PRIVATE glfw)
target_link_libraries(OpenPokemonTCG PRIVATE glm)
target_link_libraries(OpenPokemonTCG PRIVATE eventpp)
target_link_libraries(OpenPokemonTCG PRIVATE glad)
target_link_libraries(OpenPokemonTCG PRIVATE imgui::imgui)
target_link_libraries(OpenPokemonTCG PRIVATE imgui_glfw)
target_link_libraries(OpenPokemonTCG PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(OpenPokemonTCG PRIVATE ssl)
target_link_libraries(OpenPokemonTCG PRIVATE crypto)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(OpenPokemonTCG PRIVATE ${Boost_LIBRARIES})
endif()

