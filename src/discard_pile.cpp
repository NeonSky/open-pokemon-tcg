#include "discard_pile.hpp"
#include "logger.hpp"

#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg;

DiscardPile::DiscardPile(Transform transform) : transform(transform) {}

DiscardPile::~DiscardPile() {}

void DiscardPile::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  for (unsigned int i = 0; i < this->cards.size(); i++) {
    this->cards[i]->transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * i, 0.0f);
    this->cards[i]->transform.rotation = this->transform.rotation;
    this->cards[i]->render(view_projection_matrix, shader);
  }
}
