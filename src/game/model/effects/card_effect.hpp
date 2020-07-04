#pragma once

#include "../ioperate_player_side.hpp"

namespace open_pokemon_tcg::game::model {

  class ICardEffect {
  public:
    // Mutators
    virtual void activate(IOperatePlayerSide &self, IOperatePlayerSide &opponent) = 0;

    // Accessors
    virtual bool can_activate(IOperatePlayerSide &self, IOperatePlayerSide &opponent) = 0;
  };

}
