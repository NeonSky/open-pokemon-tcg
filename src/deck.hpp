#pragma once

#include "transform.hpp"
#include "card.hpp"
#include "shader.hpp"

#include <glm/mat4x4.hpp>
#include <vector>

namespace open_pokemon_tcg {

  class Deck {
  public:
    Deck(Transform transform, std::vector<Card> cards);
    ~Deck();

    Transform transform;
    std::vector<Card> cards;

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, Shader *shader);
    void shuffle();

    // Hybrids
    Card draw();
    std::vector<Card> draw(unsigned int amount);

  private:
    const float card_spacing = 0.004f;

    void update_card_positions();
  };
}
