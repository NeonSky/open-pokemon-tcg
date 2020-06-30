#pragma once

#include "attack_effect.hpp"

#include <algorithm>

namespace open_pokemon_tcg::game::model {

  class DamageFatigue : public IAttackEffect {
  public:
    DamageFatigue(IHealthTarget *self);
    ~DamageFatigue();

    // Mutators
    bool perform(unsigned int &damage) override;

  private:
    IHealthTarget *_self;
  };

  DamageFatigue::DamageFatigue(IHealthTarget *self) : _self(self) {}
  DamageFatigue::~DamageFatigue() {}

  bool DamageFatigue::perform(unsigned int &damage) {
    int reduction = _self->max_hp() - _self->hp();
    damage = std::max(0, ((int)damage) - reduction);
    return true;
  }

}
