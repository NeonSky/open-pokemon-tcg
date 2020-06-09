#include "hand.hpp"

using namespace open_pokemon_tcg;


Hand::Hand(Transform transform) : transform(transform) {}
Hand::~Hand() {}

// Mutators
void Hand::update() {
  int count = this->cards.size();
  if (count == 0)
    return;

  float mid = (1.0f + count) / 2.0f;
  float xspacing = std::min(0.8f, this->max_width / count);
  float zspacing = 0.001f;

  for (int i = 0; i < count; i++) {
    float p = i+1;
    this->cards[i].transform.position = this->transform.position + glm::vec3(xspacing * (p - mid), 0.0f, zspacing * (p - mid));
      this->cards[i].transform.rotation = this->transform.rotation;
  }
}

void Hand::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  for (auto &c : this->cards)
    c.render(view_projection_matrix, shader);
}
