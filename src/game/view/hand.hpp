#pragma once

#include "card.hpp"
#include "../model/hand.hpp"

#include <vector>

namespace open_pokemon_tcg::game::view {

  class Hand {
  public:
    Hand(model::Hand& model, engine::geometry::Transform transform);
    ~Hand();

    // Mutators
    void update();
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

    void remove(Card *card);

    // Accessors
    const model::Hand& model() const { return _model; }
    const std::vector<Card*> cards() const { return _cards; };

  private:
    const float _max_width = 7.0f; // TODO: maybe base on camera

    model::Hand& _model;
    engine::geometry::Transform _transform;
    std::vector<Card*> _cards;
  };
}
