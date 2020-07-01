#pragma once

#include "card.hpp"

namespace open_pokemon_tcg::game::model {

  class IEnergyCard : public ICard {
  public:
    virtual bool is_basic() const = 0;

    CardType card_type() const override {
      return CardType::ENERGY;
    }
  };

}
