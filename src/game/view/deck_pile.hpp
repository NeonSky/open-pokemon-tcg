#pragma once

#include "card.hpp"
#include "../model/deck_pile.hpp"

#include "../../engine/geometry/transform.hpp"
#include "../../engine/graphics/shader.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

namespace open_pokemon_tcg::game::view {

  class DeckPile {
  public:
    DeckPile(engine::geometry::Transform transform, model::DeckPile &model);
    ~DeckPile();

    engine::geometry::Transform transform;

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

  private:
    const float card_spacing = 0.004f;

    model::DeckPile& _model;
    std::vector<Card> _cards;

    // Mutators
    void update_card_positions();
  };
}
