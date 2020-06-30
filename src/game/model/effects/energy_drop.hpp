#pragma once

#include "attack_effect.hpp"
#include "../cards/traits/have_energy.hpp"

namespace open_pokemon_tcg::game::model {

  class EnergyDrop : public IAttackEffect {
  public:
    EnergyDrop(IHaveEnergy *self, EnergyAmount amount);
    ~EnergyDrop();

    // Mutators
    bool perform(unsigned int &damage) override;

  private:
    IHaveEnergy *_self;
    EnergyAmount _amount;
  };

  EnergyDrop::EnergyDrop(IHaveEnergy *self, EnergyAmount amount) : _self(self), _amount(amount){}
  EnergyDrop::~EnergyDrop() {}

  bool EnergyDrop::perform([[maybe_unused]] unsigned int &damage) {
    self->remove_energy(_amount);
    return true;
  }

}
