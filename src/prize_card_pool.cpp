#include "prize_card_pool.hpp"

using namespace open_pokemon_tcg;


PrizeCardPool::PrizeCardPool(std::array<Card*, 6> prize_cards, std::array<Transform, 6> prize_slots)
  : prize_cards(prize_cards), prize_slots(prize_slots) {

  for (unsigned int i = 0; i < this->prize_cards.size(); i++)
    this->prize_cards[i]->transform = prize_slots[i];
}

PrizeCardPool::~PrizeCardPool() {}

// Mutators
void PrizeCardPool::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  for (auto &c : prize_cards)
    c->render(view_projection_matrix, shader);
}
