#pragma once

#include "card_effect_target.hpp"
#include "../card/card.hpp"

#include <vector>

namespace open_pokemon_tcg::game::model {

  class Player;

  class IAttackEffect {
  public:
    // Mutators
    virtual void activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets, unsigned int &damage) = 0;

    // Accessors
    virtual std::vector<CardEffectTarget> required_targets() const { return {}; }
  };

}
