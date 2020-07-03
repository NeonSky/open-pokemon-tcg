#pragma once

#include "effects/card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class IPerformCardEffect {
  public:
    // Mutators
    virtual void perform(ICardEffect& effect) = 0;
  };
}
