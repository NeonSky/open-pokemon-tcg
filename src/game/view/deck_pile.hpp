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
    DeckPile(const model::DeckPile &model, engine::geometry::Transform transform);
    ~DeckPile();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

  private:
    const float card_spacing = 0.004f;

    const model::DeckPile& _model;
    engine::geometry::Transform transform;
    std::vector<Card> _cards;

    // Mutators
    void update_card_positions();
  };
}
