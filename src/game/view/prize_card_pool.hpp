#pragma once

#include "card.hpp"
#include "../model/prize_card_pool.hpp"
#include "../../engine/geometry/transform.hpp"

#include <array>

namespace open_pokemon_tcg::game::view {

  class PrizeCardPool {
  public:
    PrizeCardPool(const model::PrizeCardPool &model, std::array<engine::geometry::Transform, 6> prize_slots);
    ~PrizeCardPool();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

  private:
    const model::PrizeCardPool &_model;
    std::array<engine::geometry::Transform, 6> _prize_slots;
    std::array<std::unique_ptr<Card>, 6> _prize_cards;

  };

}
