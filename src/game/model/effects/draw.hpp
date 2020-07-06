#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class Draw : public ICardEffect {
  public:
    Draw(unsigned int amount);
    ~Draw();

    // Mutators
    void activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) override;

    // Accessors
    bool can_activate(Player &self, Player &opponent) const override;

  private:
    unsigned int _amount;
  };

}
