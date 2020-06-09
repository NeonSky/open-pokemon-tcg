#include "discard_pile.hpp"
#include "logger.hpp"

#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg;

DiscardPile::DiscardPile(Transform transform) : transform(transform) {}

DiscardPile::~DiscardPile() {}

void DiscardPile::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  for (auto &c : this->cards)
    c.render(view_projection_matrix, shader);
}

void DiscardPile::add_on_top(Card card) {
  card.transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * this->cards.size(), 0.0f);
  card.transform.rotation = this->transform.rotation;
  this->cards.push_back(card);
}

int DiscardPile::size() {
  return this->cards.size();
}
