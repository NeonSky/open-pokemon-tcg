#pragma once

#include "energy.hpp"
#include "traits/energy.hpp"

namespace open_pokemon_tcg::game::model {

  class BasicEnergy : public IEnergyCard {
  public:
    BasicEnergy(std::string id, std::string name, EnergyType energy);
    ~BasicEnergy();

    CardId id() const override;
    CardName name() const override;
    std::string to_string() const override;
    bool is_basic() const override;

    EnergyType energy_type() const;

  private:
    std::string _id;
    std::string _name;
    EnergyType _energy_type;

  };

}
