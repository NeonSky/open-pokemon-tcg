#pragma once

#include "../card.hpp"
#include "traits/energy.hpp"

namespace open_pokemon_tcg::game::model::cards {

  class EnergyCard : public ICard {
  public:
    EnergyCard(std::string id, std::string name, traits::EnergyType energy);
    ~EnergyCard();

    std::string debug() const override;
    CardId id() const override;
    CardName name() const override;

  private:
    std::string _id;
    std::string _name;
    traits::EnergyType _energy;

  };

}
