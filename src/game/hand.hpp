#pragma once

#include "card.hpp"

#include <vector>

namespace open_pokemon_tcg {

  class Hand {
  public:
    Hand(Transform transform);
    ~Hand();

    std::vector<Card*> cards;

    // Mutators
    void update();
    void render(const glm::mat4 &view_projection_matrix, Shader *shader);

    // Accessors
    unsigned int size() const { return this->cards.size(); };

  private:
    const float max_width = 7.0f; // TODO: maybe base on camera
    Transform transform;
  };
}
