#pragma once

#include "card.hpp"
#include "energy.hpp"

namespace open_pokemon_tcg::game::model {

  class EnergyCard : public ICard {
  public:
    EnergyCard(std::string id, std::string name, EnergyType energy);
    ~EnergyCard();

    std::string debug() override;
    CardId id() override;
    CardName name() override;

  private:
    std::string _id;
    std::string _name;
    EnergyType _energy;

  };

}
