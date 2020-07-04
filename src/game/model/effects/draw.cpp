#include "draw.hpp"

using namespace open_pokemon_tcg::game::model;

Draw::Draw(unsigned int amount)
  : _amount(amount) {}

Draw::~Draw() = default;

void Draw::activate(IOperatePlayerSide &self, [[maybe_unused]] IOperatePlayerSide &opponent) {
  self.draw(_amount);
}

bool Draw::can_activate(IOperatePlayerSide &self, [[maybe_unused]] IOperatePlayerSide &opponent) {
  return self.deck().cards.size() >= _amount;
}
