#pragma once

#include "card.hpp"

#include "../engine/geometry/transform.hpp"

#include <array>

namespace open_pokemon_tcg {

  class PrizeCardPool {
  public:
    PrizeCardPool(std::array<Card*, 6> prize_cards, std::array<engine::geometry::Transform, 6> prize_slots);
    ~PrizeCardPool();

    std::array<Card*, 6> prize_cards;
    std::array<engine::geometry::Transform, 6> prize_slots;

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);
  };

}
