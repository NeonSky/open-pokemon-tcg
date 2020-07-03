#include "prize_card_pool.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::view;


PrizeCardPool::PrizeCardPool(const model::PrizeCardPool &model, std::array<engine::geometry::Transform, 6> prize_slots)
  : _model(model),
    _prize_slots(prize_slots) {

  for (unsigned int i = 0; i < _prize_cards.size(); i++)
    _prize_cards[i] = std::make_unique<Card>(*_model.cards()[i], _prize_slots[i]);

  _model.on_take([this](unsigned int index) {
    _prize_cards[index].reset();
  });
}

PrizeCardPool::~PrizeCardPool() = default;

// Mutators
void PrizeCardPool::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (auto &c : _prize_cards)
    if (c != nullptr)
      c->render(view_projection_matrix, shader);
}
