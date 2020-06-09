#include "deck.hpp"
#include "logger.hpp"

#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg;

Deck::Deck(Transform transform, std::vector<Card> cards) : transform(transform) {
  for (auto &c : cards)
    add_on_top(c);
}

Deck::~Deck() {}

void Deck::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  for (auto &c : this->cards)
    c.render(view_projection_matrix, shader);
}

Card Deck::draw() {
  if (this->cards.empty())
    LOG_ERROR("Can not draw card from empty deck.");

  Card card = this->cards.back();
  this->cards.pop_back();

  return card;
}

std::vector<Card> Deck::draw(unsigned int amount) {
  if (this->cards.size() < amount)
    LOG_ERROR("Can not draw " + std::to_string(amount) + " cards from deck with " + std::to_string(cards.size()) + " cards.");

  std::vector<Card> cx;
  for (unsigned int i = 0; i < amount; i++) {
    cx.push_back(this->cards.back());
    this->cards.pop_back();
  }

  return cx;
}

void Deck::shuffle() {
  random_shuffle(this->cards.begin(), this->cards.end());
}

void Deck::add_on_top(Card card) {
  card.transform.position = this->transform.position + glm::vec3(0.0f, this->card_spacing * this->cards.size(), 0.0f);
  card.transform.rotation = this->transform.rotation + glm::vec3(0.0f, 0.0f, glm::pi<float>());
  this->cards.push_back(card);
}

int Deck::size() {
  return this->cards.size();
}
