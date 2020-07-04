#pragma once

#include "../player.hpp"

namespace open_pokemon_tcg::game::model {

  class ICardEffect {
  public:
    // Mutators
    virtual void activate(Player &self, Player &opponent) = 0;

    // Accessors
    virtual bool can_activate(Player &self, Player &opponent) = 0; // TODO: struct as response with reason.
  };

}
