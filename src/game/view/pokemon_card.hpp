#pragma once

#include "card.hpp"
#include "../model/card/pokemon.hpp"

namespace open_pokemon_tcg::game::view {
  class PokemonCard {
  public:
    PokemonCard(game::model::PokemonCard &model, engine::geometry::Transform transform);
    ~PokemonCard();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

    // Accessors
    glm::mat4 model_matrix() const;
    engine::geometry::Intersection* does_intersect(engine::geometry::Ray ray) const;
    const engine::graphics::Texture& texture() const;
    const std::vector<Card*> energy_views() const;

  private:
    game::model::PokemonCard& _model;
    Card& _pokemon_view;
    std::vector<Card*> _energy_views;

    void update_transforms();

  public:
    engine::geometry::Transform transform; // FIXME. Maybe a pub ref function?

  };
}
