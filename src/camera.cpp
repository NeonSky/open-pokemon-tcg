#include "camera.hpp"

// TODO: only when debugging, move to lib
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace open_pokemon_tcg;

Camera::Camera() : orientation(Orientation()){
  this->pos = glm::vec3(0.0f, 0.0f, -1.0f);
}

Camera::Camera(Orientation orientation) : orientation(orientation){
  this->pos = glm::vec3(0.0f, 0.0f, -1.0f);
}

Camera::~Camera() {}

void Camera::move(Direction move_dir) {
  glm::vec3 displacement_dir;
  switch (move_dir) {
  case Direction::FORWARD:
    displacement_dir = this->orientation.forward();
    break;
  case Direction::BACKWARD:
    displacement_dir = -this->orientation.forward();
    break;
  case Direction::RIGHT:
    displacement_dir = this->orientation.right();
    break;
  case Direction::LEFT:
    displacement_dir = -this->orientation.right();
    break;
  case Direction::UP:
    displacement_dir = this->orientation.up();
    break;
  case Direction::DOWN:
    displacement_dir = -this->orientation.up();
    break;
  default:
    // std::cerr << "Direction not supported" << std::endl;
    assert(false); // TODO: change to error
  }
  this->pos += this->movement_speed * displacement_dir;
  std::cout << "Camera now at: " << glm::to_string(this->pos) << std::endl;
}

void Camera::look_at(glm::vec3 target) {
  this->orientation = Orientation(target - this->pos);
}

bool firstMouse = true;
double lastX;
double lastY;
double yaw = -90.0f;
double pitch = 0.0f;

// TODO: Maybe move to player_camera?
void Camera::lookat_mouse(float mouse_xpos, float mouse_ypos) {
  if (firstMouse)
    {
      lastX = mouse_xpos;
      lastY = mouse_ypos;
      firstMouse = false;
    }

  float xoffset = mouse_xpos - lastX;
  float yoffset = lastY - mouse_ypos;
  lastX = mouse_xpos;
  lastY = mouse_ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch = 89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  this->orientation = Orientation(glm::normalize(direction));
}

glm::mat4 Camera::view_matrix() const {
  glm::mat3 base_vectors_in_world_space(
    this->orientation.right(),
    this->orientation.up(),
    -this->orientation.forward()
  );
  // NOTE: transpose = inverse, since the matrix is an orthonormal base.
  glm::mat3 inverse_base = glm::transpose(base_vectors_in_world_space);

  return glm::mat4(inverse_base) * glm::translate(-this->pos);
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
    // std::cerr << "Projection type not supported" << std::endl;
    assert(false); // TODO: change to error
  }
}
