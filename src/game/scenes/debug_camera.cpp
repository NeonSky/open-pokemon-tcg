#include "debug_camera.hpp"

#include "../../engine/debug/logger.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <algorithm>

using namespace open_pokemon_tcg::game::scenes;

DebugCamera::DebugCamera(engine::gui::Window* window) : camera(engine::graphics::Camera()), window(window){}
DebugCamera::DebugCamera(engine::gui::Window* window, engine::geometry::Transform transform) : camera(engine::graphics::Camera(transform)), window(window) {
  this->window->add_on_key_callback(std::bind(&DebugCamera::on_key, this, std::placeholders::_1));
  this->window->add_on_mouse_move_callback(std::bind(&DebugCamera::on_cursor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  this->window->add_on_mouse_scroll_callback(std::bind(&DebugCamera::on_scroll, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

DebugCamera::~DebugCamera() {}

// Mutators
void DebugCamera::set_transform(engine::geometry::Transform transform) {
  this->camera.transform = transform;
};

void DebugCamera::on_key(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W))
    this->camera.move(engine::graphics::Direction::FORWARD);
  if (glfwGetKey(window, GLFW_KEY_S))
    this->camera.move(engine::graphics::Direction::BACKWARD);
  if (glfwGetKey(window, GLFW_KEY_D))
    this->camera.move(engine::graphics::Direction::RIGHT);
  if (glfwGetKey(window, GLFW_KEY_A))
    this->camera.move(engine::graphics::Direction::LEFT);
  if (glfwGetKey(window, GLFW_KEY_E))
    this->camera.move(engine::graphics::Direction::UP);
  if (glfwGetKey(window, GLFW_KEY_Q))
    this->camera.move(engine::graphics::Direction::DOWN);

  if (glfwGetKey(window, GLFW_KEY_P)) {
    if (this->projection_type == engine::graphics::ProjectionType::PERSPECTIVE)
      this->projection_type = engine::graphics::ProjectionType::ORTHOGRAPHIC;
    else
      this->projection_type = engine::graphics::ProjectionType::PERSPECTIVE;
  }

  if (glfwGetKey(window, GLFW_KEY_T)) {
    this->free_look_mode = !this->free_look_mode;
    if (this->free_look_mode)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void DebugCamera::on_cursor(GLFWwindow* window, float xpos, float ypos) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  // Viewport Space [0:width, 0:height]
  float x = xpos;
  float y = height - ypos; // In OpenGL, (0, 0) is top-left. We want (0, 0) to be bot-left.

  // Normalized Device Space [-1:1, -1:1, -1:1]
  glm::vec3 ndc(2.0f * (x / width) - 1.0f, 2.0f * (y / height) - 1.0f, -1.0f); // z = -1.0f since cursor points along camera's forward direction.

  // Homogeneous Clip Space [-1:1, -1:1, -1:1, -1:1]
  glm::vec4 clip_coords(ndc.x, ndc.y, ndc.z, 1.0f);

  // View Space (Eye Space) [-inf:inf, -inf:inf, -inf:inf, -inf:inf]
  glm::vec4 view_coords(glm::vec3(glm::inverse(projection_matrix()) * clip_coords), 0.0f);

  // World Space [-inf:inf, -inf:inf, -inf:inf, -inf:inf]
  glm::vec4 world_coords = glm::inverse(view_matrix()) * view_coords;

  // Normalized 3D vector form
  this->_mouse_ray = glm::normalize(glm::vec3(world_coords));

  if (this->free_look_mode)
    this->camera.lookat_mouse(x, y);
}

void DebugCamera::on_scroll([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] float xoffset, float yoffset) {
  this->zoom = std::clamp(this->zoom + yoffset, this->zoom_min, this->zoom_max);
  this->camera.set_zoom(this->zoom);
}
