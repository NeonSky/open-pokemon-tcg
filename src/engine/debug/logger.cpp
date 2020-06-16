#include "logger.hpp"

#include <string>
#include <glad/glad.h>

using namespace open_pokemon_tcg::engine::debug;

bool checkGLError(const char* file, int line) {
	bool wasError = false;
	for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError()) {
    LOG_ERROR("OpenGL error #" + std::to_string(glErr) + " in " + file + " at line: " + std::to_string(line));
    wasError = true;
  }
	return wasError;
}

void Logger::set_profile(Logger::Profile profile) {
  switch (profile) {
  case Profile::DEBUG:
    apply_debug_profile();
    break;
  case Profile::DEV:
    apply_dev_profile();
    break;
  case Profile::RELEASE:
    apply_release_profile();
    break;
  }
}

void Logger::apply_debug_profile() {
  spdlog::set_level(spdlog::level::debug);
}

void Logger::apply_dev_profile() {
  spdlog::set_level(spdlog::level::info);
}

void Logger::apply_release_profile() {
  spdlog::set_level(spdlog::level::err);
}
