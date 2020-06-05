#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <cstdlib>

#include "window.hpp"
#include "card.hpp"
#include "texture.hpp"
#include "scenes/debug/card_rotation.hpp"

using namespace open_pokemon_tcg;

bool checkGLError(const char* file, int line) {
	bool wasError = false;
	for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError()) {
      std::cerr << "GL Error #" << glErr << " in " << file << " at line: " << line << std::endl;
      wasError = true;
    }
	return wasError;
}

#define CHECK_GL_ERROR()                                      \
	{                                                          \
		checkGLError(__FILE__, __LINE__);  \
	}

void print_system_info() {
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
}

void gui(IScene* scene) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  scene->gui();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
  auto start = std::chrono::system_clock::now();
  std::time_t start_time = std::chrono::system_clock::to_time_t(start);
  std::cout << "### Program started at: " << std::ctime(&start_time);

  Window *window;
  try {
    window = new Window(1920/2, 1080, "OpenPokemonTCG");
  } catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  if (!status) {
    std::cout << "Failed to init GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  print_system_info();

  IScene* scene = new CardRotation(window);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  window->init_gui();

  CHECK_GL_ERROR();
  while (!window->is_closing()) {
    window->clear_screen();

    CHECK_GL_ERROR();
    scene->update();
    CHECK_GL_ERROR();
    scene->render();
    CHECK_GL_ERROR();

    glUseProgram(0);
    gui(scene);

    window->update();
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Program terminated at: " << std::ctime(&end_time);
  std::cout << "Uptime: " << elapsed_seconds.count() << " seconds.\n";
}

