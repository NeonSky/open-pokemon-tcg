#include "orientation.hpp"

#include <glm/ext/matrix_transform.hpp>

using namespace open_pokemon_tcg;

Orientation::Orientation() {
  this->_forward = glm::vec3(0.0f, 0.0f, -1.0f);
  this->_right = glm::normalize(glm::cross(this->_forward, this->world_up));
  this->_up = glm::normalize(glm::cross(this->_right, this->_forward));
}

Orientation::Orientation(glm::vec3 forward) {
  this->_forward = forward;
  this->_right = glm::normalize(glm::cross(this->_forward, this->world_up));
  this->_up = glm::normalize(glm::cross(this->_right, this->_forward));
}

Orientation::~Orientation() {}
