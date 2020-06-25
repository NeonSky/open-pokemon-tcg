#pragma once

#include "effects/card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class IGameMaster {
  public:
    // Mutators
    virtual void activate(effects::ICardEffect& effect) = 0;
  };
}
