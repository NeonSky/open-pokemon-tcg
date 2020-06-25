#include "deck_pile.hpp"

#include "../../engine/debug/logger.hpp"

#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg::game::view;

DeckPile::DeckPile(engine::geometry::Transform transform, std::vector<Card*> cards) : transform(transform), cards(cards) {
  update_card_positions();
}

DeckPile::~DeckPile() {}

void DeckPile::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (auto &c : this->cards)
    c->render(view_projection_matrix, shader);
}

Card* DeckPile::draw() {
  if (this->cards.empty())
    LOG_ERROR("Can not draw card from empty deck.");

  Card *card = this->cards.back();
  this->cards.pop_back();

  return card;
}

std::vector<Card*> DeckPile::draw(unsigned int amount) {
  if (this->cards.size() < amount)
    LOG_ERROR("Can not draw " + std::to_string(amount) + " cards from deck with " + std::to_string(cards.size()) + " cards.");

  std::vector<Card*> cx;
  for (unsigned int i = 0; i < amount; i++) {
    cx.push_back(this->cards.back());
    this->cards.pop_back();
  }

  return cx;
}

void DeckPile::shuffle() {
  random_shuffle(this->cards.begin(), this->cards.end());
  update_card_positions();
}

void DeckPile::update_card_positions() {
  for (unsigned int h = 0; h < this->cards.size(); h++) {
    this->cards[h]->transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * h, 0.0f);
    this->cards[h]->transform.rotation = this->transform.rotation + glm::vec3(-glm::pi<float>(), -glm::pi<float>(), 0.0f);
  }
}
