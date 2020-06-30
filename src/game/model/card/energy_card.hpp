#pragma once

#include "../card.hpp"
#include "traits/energy.hpp"

namespace open_pokemon_tcg::game::model {

  class EnergyCard : public ICard {
  public:
    EnergyCard(std::string id, std::string name, EnergyType energy);
    ~EnergyCard();

    std::string to_string() const override;
    CardId id() const override;
    CardName name() const override;

  private:
    std::string _id;
    std::string _name;
    EnergyType _energy;

  };

}
