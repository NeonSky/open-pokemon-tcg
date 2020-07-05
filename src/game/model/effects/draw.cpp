#include "draw.hpp"

using namespace open_pokemon_tcg::game::model;

Draw::Draw(unsigned int amount)
  : _amount(amount) {}

Draw::~Draw() = default;

void Draw::activate(Player &self, [[maybe_unused]] Player &opponent) {
  self.draw(_amount);
}

bool Draw::can_activate(Player &self, [[maybe_unused]] Player &opponent) const {
  return self.deck().cards.size() >= _amount;
}
