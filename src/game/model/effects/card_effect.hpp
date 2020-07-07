#pragma once

#include "card_effect_target.hpp"
#include "../player.hpp"
#include "../card/card.hpp"

#include <vector>

namespace open_pokemon_tcg::game::model {

  class ICardEffect {
  public:
    // Mutators
    virtual void activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) = 0;

    // Accessors
    virtual std::vector<CardEffectTarget> required_targets() const { return {}; }
    virtual bool can_activate([[maybe_unused]] Player &self, [[maybe_unused]] Player &opponent) const { return true; }; // TODO: struct as response with reason.
  };

}
