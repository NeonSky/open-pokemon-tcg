#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class Draw : public ICardEffect {
  public:
    Draw(unsigned int amount);
    ~Draw();

    // Mutators
    void activate(IOperatePlayerSide &self, IOperatePlayerSide &opponent) override;

    // Accessors
    bool can_activate(IOperatePlayerSide &self, IOperatePlayerSide &opponent) override;

  private:
    unsigned int _amount;
  };

}
