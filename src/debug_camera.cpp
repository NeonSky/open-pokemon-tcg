#include "debug_camera.hpp"
#include <algorithm>
#include <iostream>

using namespace open_pokemon_tcg;

DebugCamera::DebugCamera(Window* window) : camera(Camera()), window(window){}
DebugCamera::DebugCamera(Window* window, Transform transform) : camera(Camera(transform)), window(window) {
  this->window->add_on_key_callback(std::bind(&DebugCamera::on_key, this, std::placeholders::_1));
  this->window->add_on_cursor_callback(std::bind(&DebugCamera::on_cursor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->window->add_on_scroll_callback(std::bind(&DebugCamera::on_scroll, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

DebugCamera::~DebugCamera() {}

// Mutators
void DebugCamera::on_key(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W))
    this->camera.move(Direction::FORWARD);
  if (glfwGetKey(window, GLFW_KEY_S))
    this->camera.move(Direction::BACKWARD);
  if (glfwGetKey(window, GLFW_KEY_D))
    this->camera.move(Direction::RIGHT);
  if (glfwGetKey(window, GLFW_KEY_A))
    this->camera.move(Direction::LEFT);
  if (glfwGetKey(window, GLFW_KEY_E))
    this->camera.move(Direction::UP);
  if (glfwGetKey(window, GLFW_KEY_Q))
    this->camera.move(Direction::DOWN);

  if (glfwGetKey(window, GLFW_KEY_P)) {
    if (this->projection_type == ProjectionType::PERSPECTIVE)
      this->projection_type = ProjectionType::ORTHOGRAPHIC;
    else
      this->projection_type = ProjectionType::PERSPECTIVE;
  }

  if (glfwGetKey(window, GLFW_KEY_T)) {
    this->free_look_mode = !this->free_look_mode;
    if (this->free_look_mode)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void DebugCamera::on_cursor([[maybe_unused]] GLFWwindow* window, float xpos, float ypos) {
  if (this->free_look_mode)
    this->camera.lookat_mouse(xpos, ypos);
}

void DebugCamera::on_scroll([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] float xoffset, float yoffset) {
  this->zoom = std::clamp(this->zoom + yoffset, this->zoom_min, this->zoom_max);
  this->camera.set_zoom(this->zoom);
}
