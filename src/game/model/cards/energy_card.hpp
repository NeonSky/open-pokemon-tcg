#pragma once

#include "../card.hpp"
#include "traits/energy.hpp"

namespace open_pokemon_tcg::game::model::cards {

  class EnergyCard : public ICard {
  public:
    EnergyCard(std::string id, std::string name, traits::EnergyType energy);
    ~EnergyCard();

    std::string debug() override;
    CardId id() override;
    CardName name() override;

  private:
    std::string _id;
    std::string _name;
    traits::EnergyType _energy;

  };

}
