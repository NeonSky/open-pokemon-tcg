#include "discard_pile.hpp"

using namespace open_pokemon_tcg;

DiscardPile::DiscardPile(const engine::geometry::Transform& transform) : transform(transform) {}

DiscardPile::~DiscardPile() = default;

void DiscardPile::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (unsigned int i = 0; i < this->cards.size(); i++) {
    this->cards[i]->transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * i, 0.0f);
    this->cards[i]->transform.rotation = this->transform.rotation;
    this->cards[i]->render(view_projection_matrix, shader);
  }
}
