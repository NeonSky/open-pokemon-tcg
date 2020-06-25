#include "hand.hpp"

#include "../../engine/debug/logger.hpp"

#include <algorithm>

using namespace open_pokemon_tcg::game::view;


Hand::Hand(engine::geometry::Transform transform) : transform(transform) {}
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
    this->cards[i]->transform.position = this->transform.position + glm::vec3(xspacing * (p - mid), 0.0f, zspacing * (p - mid));
    this->cards[i]->transform.rotation = this->transform.rotation;
  }
}

void Hand::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (auto &c : this->cards)
    c->render(view_projection_matrix, shader);
}

void Hand::remove(Card *card) {
  std::vector<Card*>::iterator it = std::find(this->cards.begin(), this->cards.end(), card);
  if (it == this->cards.end())
    LOG_ERROR("Tried to remove card from hand, but that card was not found.");

  this->cards.erase(it);
}
