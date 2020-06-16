#include "camera.hpp"
#include "../debug/logger.hpp"

#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace open_pokemon_tcg::engine::graphics;

// FIXME
bool firstMouse = true;
double lastX;
double lastY;
double yaw = 0.0f;
double pitch = 0.0f;

Camera::Camera() : Camera(geometry::Transform()){}
Camera::Camera(const geometry::Transform& transform) : transform(transform){
  yaw = this->transform.yaw();
  pitch = this->transform.pitch();
}

Camera::~Camera() = default;

void Camera::move(Direction move_dir) {
  glm::vec3 displacement_dir;
  switch (move_dir) {
  case Direction::FORWARD:
    displacement_dir = this->transform.forward();
    break;
  case Direction::BACKWARD:
    displacement_dir = -this->transform.forward();
    break;
  case Direction::RIGHT:
    displacement_dir = this->transform.right();
    break;
  case Direction::LEFT:
    displacement_dir = -this->transform.right();
    break;
  case Direction::UP:
    displacement_dir = this->transform.up();
    break;
  case Direction::DOWN:
    displacement_dir = -this->transform.up();
    break;
  default:
    LOG_ERROR("Direction type not supported: " + std::to_string(move_dir));
  }
  this->transform.position += this->movement_speed * displacement_dir;
}

// void Camera::look_at(glm::vec3 target) {
//   this->orientation = Orientation(target - this->pos);
// }

void Camera::set_zoom(float zoom_level) {
  // FIXME: name constant
  this->perspective.fov = 45.0f - zoom_level;
}

void Camera::lookat_mouse(float mouse_xpos, float mouse_ypos) {
  if (firstMouse)
    {
      lastX = mouse_xpos;
      lastY = mouse_ypos;
      firstMouse = false;
    }

  float xoffset = mouse_xpos - lastX;
  float yoffset = mouse_ypos - lastY;
  lastX = mouse_xpos;
  lastY = mouse_ypos;

  float sensitivity = 0.003f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   -= xoffset;
  pitch += yoffset;

  pitch = std::clamp(pitch, -glm::half_pi<double>(), glm::half_pi<double>());

  this->transform.set_rotation(yaw, pitch, 0.0f);
}

glm::mat4 Camera::view_matrix() const {
  glm::mat3 base_vectors_in_world_space(this->transform.right(), this->transform.up(), -this->transform.forward());

  // NOTE: transpose = inverse, since the matrix is an orthonormal base.
  glm::mat3 inverse_base = glm::transpose(base_vectors_in_world_space);

  return glm::mat4(inverse_base) * glm::translate(-this->transform.position);
}

glm::mat4 Camera::projection_matrix(ProjectionType projection_type) const {
  switch (projection_type) {
  case ProjectionType::PERSPECTIVE:
    return glm::perspective(
      glm::radians(this->perspective.fov),
      this->perspective.aspect_ratio,
      this->perspective.near,
      this->perspective.far
    );
  case ProjectionType::ORTHOGRAPHIC:
    return glm::ortho(
      this->orthographic.left,
      this->orthographic.right,
      this->orthographic.bot,
      this->orthographic.top,
      this->orthographic.near,
      this->orthographic.far
    );
  default:
    LOG_ERROR("Projection type not supported: " + std::to_string(projection_type));
    assert(false);
  }
}
