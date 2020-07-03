#include "deck_pile.hpp"

#include "../../engine/debug/logger.hpp"

#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg::game::view;

DeckPile::DeckPile(const model::DeckPile &model, engine::geometry::Transform transform)
  : _model(model),
    transform(transform) {

  for (auto card : _model.cards())
    _cards.push_back(Card(card, engine::geometry::Transform()));
  update_card_positions();

  _model.on_pop([this]() {
    _cards.pop_back();
    update_card_positions();
  });
}

DeckPile::~DeckPile() {}

void DeckPile::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (auto &c : _cards)
    c.render(view_projection_matrix, shader);
}

void DeckPile::update_card_positions() {
  for (unsigned int h = 0; h < _cards.size(); h++) {
    _cards[h].transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * h, 0.0f);
    _cards[h].transform.rotation = this->transform.rotation + glm::vec3(-glm::pi<float>(), -glm::pi<float>(), 0.0f);
  }
}
