#include "discard_pile.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::view;

DiscardPile::DiscardPile(const model::DiscardPile &model, const engine::geometry::Transform& transform)
  : _model(model),
    transform(transform) {

  _model.on_push([this](model::ICard& card) {
    _cards.push_back(Card(card, engine::geometry::Transform()));
  });
}

DiscardPile::~DiscardPile() = default;

void DiscardPile::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (unsigned int i = 0; i < _cards.size(); i++) {
    _cards[i].transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * i, 0.0f);
    _cards[i].transform.rotation = this->transform.rotation;
    _cards[i].render(view_projection_matrix, shader);
  }
}
