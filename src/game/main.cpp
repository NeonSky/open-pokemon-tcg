#include "scenes/duel.hpp"
#include "scenes/debug/card_transform.hpp"
#include "scenes/debug/deck_loading.hpp"
#include "scenes/debug/playmat_slots.hpp"

#include "../engine/debug/logger.hpp"
#include "../engine/gui/window.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>


using namespace open_pokemon_tcg;

void gui(IScene* scene) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  scene->gui();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
  srand(time(NULL));

  Logger::set_profile(Logger::Profile::DEBUG);
  LOG_INFO("Program started.");
  auto start = std::chrono::system_clock::now();

  Window *window;
  try {
    window = new Window(1920/2, 1080, "OpenPokemonTCG");
  } catch(const std::exception& e) {
    LOG_ERROR(e.what());
    return -1;
  }

  int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  if (!status) {
    LOG_ERROR("Failed to init GLAD");
    glfwTerminate();
    return -1;
  }

  LOG_INFO("OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));
  LOG_INFO("OpenGL renderer: " + std::string((const char*)glGetString(GL_RENDERER)));
  LOG_INFO("OpenGL vendor: " + std::string((const char*)glGetString(GL_VENDOR)));

  CHECK_GL_ERROR();
  // IScene* scene = new scenes::Duel(window);
  // IScene* scene = new scenes::CardTransform(window);
  // IScene* scene = new scenes::DeckLoading(window);
  IScene* scene = new scenes::PlaymatSlots(window);

  CHECK_GL_ERROR();

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
  LOG_INFO("Program terminated.");

  std::chrono::duration<double> elapsed_seconds = end - start;
  LOG_INFO("Uptime: " + std::to_string(elapsed_seconds.count()) + " seconds.");
}

