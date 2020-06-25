#pragma once

#include "card.hpp"

#include "../../engine/geometry/rectangle.hpp"

#include <array>

namespace open_pokemon_tcg::game::view {

  class Bench {
  public:
    Bench(std::array<engine::geometry::Rectangle, 5> slots);
    ~Bench();

    // Mutators
    void place(Card *card);
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

  private:
    std::array<engine::geometry::Rectangle, 5> slots;
    std::array<Card*, 5> cards;
  };

}
