#include "window.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <stdexcept>

using namespace open_pokemon_tcg;

Window::Window() : Window(1920, 1080, "OpenPokemonTCG") {}

Window::Window(int width, int height, const char* title) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to init glfw");
  }

  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create glfw window");
  }

  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

  glfwSetKeyCallback(window, [](GLFWwindow* window, [[maybe_unused]] int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods) {
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    for (std::function<void(GLFWwindow*)> c : w->on_key_callbacks)
      c(window);
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    for (std::function<void(GLFWwindow*, float, float)> c : w->on_cursor_callbacks)
      c(window, xpos, ypos);
  });

  glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    for (std::function<void(GLFWwindow*, float, float)> c : w->on_scroll_callbacks)
      c(window, xoffset, yoffset);
  });

  this->window = window;
}

void Window::init_gui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  std::string glsl_version = "#version 420";
  ImGui_ImplGlfw_InitForOpenGL(this->window, true);
  ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

Window::~Window() {
  glfwTerminate();
}

void Window::update() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Window::clear_screen() {
  glClearColor(0.2f, 0.2f, 0.8f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::is_closing() {
  return glfwWindowShouldClose(this->window);
}

void Window::add_on_key_callback(std::function<void(GLFWwindow*)> callback) {
  this->on_key_callbacks.push_back(callback);
}

void Window::add_on_cursor_callback(std::function<void(GLFWwindow*, float, float)> callback) {
  this->on_cursor_callbacks.push_back(callback);
}

void Window::add_on_scroll_callback(std::function<void(GLFWwindow*, float, float)> callback) {
  this->on_scroll_callbacks.push_back(callback);
}
