#include "bench.hpp"

#include "../engine/debug/logger.hpp"

using namespace open_pokemon_tcg;

Bench::Bench(std::array<engine::geometry::Rectangle, 5> slots) : slots(slots) {
  for (unsigned int i = 0; i < this->cards.size(); i++)
    this->cards[i] = nullptr;
}
Bench::~Bench() {}

// Mutators
void Bench::place(Card *card) {
  for (unsigned int i = 0; i < this->cards.size(); i++) {
    if (this->cards[i] == nullptr) {
      this->cards[i] = card;
      this->cards[i]->transform = this->slots[i].transform();
      return;
    }
  }

  LOG_ERROR("Tried to place card on bench, but no place was available.");
}

void Bench::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (Card* c : this->cards)
    if (c != nullptr)
      c->render(view_projection_matrix, shader);
}
