#include "rectangle.hpp"

using namespace open_pokemon_tcg::engine::geometry;

Rectangle::Rectangle() : Rectangle(Transform()) {}
Rectangle::Rectangle(Transform transform, float width, float height) : _transform(transform), _width(width), _height(height) {
  update_corners();
}

// Accessors
Transform Rectangle::transform() const { return this->_transform; }
float Rectangle::width() const { return this->_width; }
float Rectangle::height() const { return this->_height; }

glm::vec3 Rectangle::topleft() const { return this->_topleft; }
glm::vec3 Rectangle::topright() const { return this->_topright; }
glm::vec3 Rectangle::botleft() const { return this->_botleft; }
glm::vec3 Rectangle::botright() const { return this->_botright; }

// Mutators
void Rectangle::update_corners() {
  glm::vec3 xoffset = (this->_width/2.0f) * Transform::world_right;
  glm::vec3 yoffset = (this->_height/2.0f) * Transform::world_up;

  this->_topleft  = this->_transform.matrix() * glm::vec4(+xoffset + yoffset, 1.0f);
  this->_topright = this->_transform.matrix() * glm::vec4(-xoffset + yoffset, 1.0f);
  this->_botleft  = this->_transform.matrix() * glm::vec4(+xoffset - yoffset, 1.0f);
  this->_botright = this->_transform.matrix() * glm::vec4(-xoffset - yoffset, 1.0f);
}
