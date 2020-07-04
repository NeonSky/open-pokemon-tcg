#pragma once

#include "deck.hpp"

namespace open_pokemon_tcg::game::model {

  class IOperatePlayerSide {
  public:
    // Mutators
    virtual void draw(unsigned int amount = 1) = 0;
    virtual void place_active_pokemon(unsigned int hand_index) = 0;

    // Accessors
    virtual const Deck& deck() const = 0;
  };

}
