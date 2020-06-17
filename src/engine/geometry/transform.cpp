#include "transform.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

using namespace open_pokemon_tcg::engine::geometry;

Transform::Transform() : Transform(glm::vec3(0.0f)) {}
Transform::Transform(glm::vec3 position) : Transform(position, glm::vec3(0.0f)) {}
Transform::Transform(glm::vec3 position, glm::vec3 rotation) : Transform(position, rotation, glm::vec3(1.0f)) {}
Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
  this->position = position;
  this->rotation = rotation;
  this->scale = scale;
}

Transform::~Transform() {}

void Transform::set_rotation(float yaw, float pitch, float roll) {
  this->rotation = glm::vec3(pitch, yaw, roll);
}

void Transform::flip_rotation() {
  glm::mat4 m = glm::rotate(rotation_matrix(), glm::pi<float>(), glm::vec3(this->world_up));

  // See: https://github.com/jzrake/glm/blob/d3313421c664db5bd1b672d39ba3faec0d430117/glm/gtx/euler_angles.inl#L213
  // NOTE: we negate each angle due to rotation_matrix() negating the angles.
  float yaw   = -std::atan2(m[2][0], m[2][2]);
  float pitch = -std::asin(-m[2][1]);
  float roll  = -std::atan2(m[0][1], m[1][1]);

  set_rotation(yaw, pitch, roll);
}

Transform Transform::operator+(const Transform &other) const {
  return Transform(this->position + other.position, this->rotation + other.rotation, this->scale + other.scale);
}

glm::mat4 Transform::matrix() const {
  glm::mat4 m = glm::mat4(1.0f);

  // Scale -> Rotate -> Translate
  m = glm::scale(m, this->scale);
  m = rotation_matrix() * m;
  m = glm::translate(glm::mat4(1.0f), this->position) * m;

  return m;
}

glm::vec3 Transform::forward() const {
  return rotation_matrix() * this->world_forward;
}

glm::vec3 Transform::up() const {
  return rotation_matrix() * this->world_up;
}

glm::vec3 Transform::right() const {
  return rotation_matrix() * this->world_right;
}

// https://www.wikiwand.com/en/Rotation_matrix
// https://www.wikiwand.com/en/Euler_angles
// https://www.wikiwand.com/simple/Pitch,_yaw,_and_roll
//
// Here we apply y-rotation (yaw), then x-rotation (pitch), and finally z-rotation (roll).
glm::mat4 Transform::rotation_matrix() const {
  return glm::eulerAngleYXZ(-this->rotation.y, -this->rotation.x, -this->rotation.z);
}
