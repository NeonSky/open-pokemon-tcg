#pragma once

#include "card.hpp"
#include "../model/discard_pile.hpp"

#include "../../engine/geometry/transform.hpp"
#include "../../engine/graphics/shader.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

namespace open_pokemon_tcg::game::view {

  class DiscardPile {
  public:
    DiscardPile(const model::DiscardPile &model, const engine::geometry::Transform& transform);
    ~DiscardPile();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

    // Accessors
    const std::vector<Card> cards() const { return _cards; };

  private:
    const float card_spacing = 0.004f;

    const model::DiscardPile &_model;
    engine::geometry::Transform transform;
    std::vector<Card> _cards;
  };
}
