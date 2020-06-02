#include "window.hpp"

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
  this->window = window;
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

void Window::add_on_key_callback(GLFWkeyfun callback) {
  glfwSetKeyCallback(window, callback);
}

void Window::add_on_cursor_callback(GLFWcursorposfun callback) {
  glfwSetCursorPosCallback(window, callback);
}

void Window::add_on_scroll_callback(GLFWscrollfun callback) {
  glfwSetScrollCallback(window, callback);
}
