#pragma once

#include "../player.hpp"
#include "../card/card.hpp"

#include <vector>

namespace open_pokemon_tcg::game::model {

  enum CardEffectTarget {
    POKEMON,
    ENEMY_POKEMON,
    FRIENDLY_POKEMON,
    ENERGY,
    ENERGY_FROM_PREVIOUS_POKEMON,
    ENEMY_ENERGY,
    FRIENDLY_ENERGY,
  };

  class ICardEffect {
  public:
    // Mutators
    virtual void set_targets([[maybe_unused]] std::vector<std::reference_wrapper<const ICard>> targets) {}
    virtual void activate(Player &self, Player &opponent) = 0;

    // Accessors
    virtual std::vector<CardEffectTarget> required_targets() const { return {}; }
    virtual bool can_activate(Player &self, Player &opponent) const = 0; // TODO: struct as response with reason.
  };

}
