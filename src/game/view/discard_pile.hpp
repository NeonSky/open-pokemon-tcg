#pragma once

#include "card.hpp"

#include "../../engine/geometry/transform.hpp"
#include "../../engine/graphics/shader.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

namespace open_pokemon_tcg::game::view {

  class DiscardPile {
  public:
    DiscardPile(const engine::geometry::Transform& transform);
    ~DiscardPile();

    engine::geometry::Transform transform;
    std::vector<Card*> cards;

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

  private:
    const float card_spacing = 0.004f;
  };
}
