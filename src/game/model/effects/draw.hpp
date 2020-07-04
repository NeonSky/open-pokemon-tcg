#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class Draw : public ICardEffect {
  public:
    Draw(unsigned int amount);
    ~Draw();

    // Mutators
    void activate(Player &self, Player &opponent) override;

    // Accessors
    bool can_activate(Player &self, Player &opponent) override;

  private:
    unsigned int _amount;
  };

}
