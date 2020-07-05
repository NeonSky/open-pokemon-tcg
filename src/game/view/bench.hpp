#pragma once

#include "pokemon_card.hpp"
#include "../model/bench.hpp"
#include "../../engine/geometry/rectangle.hpp"

#include <array>

namespace open_pokemon_tcg::game::view {

  class Bench {
  public:
    Bench(model::Bench &model, std::array<engine::geometry::Rectangle, 5> slots);
    ~Bench();

    // Mutators
    void render(const glm::mat4& view_projection_matrix, engine::graphics::Shader* shader);

  private:
    model::Bench& _model;
    std::array<engine::geometry::Rectangle, 5> _slots;
    std::array<PokemonCard*, 5> _cards;
  };

}
