#pragma once

#include "attack_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class SelfDamage : public IAttackEffect {
  public:
    SelfDamage(IHealthTarget *self, unsigned int amount);
    ~SelfDamage();

    // Mutators
    bool perform(unsigned int &damage) override;

  private:
    IHealthTarget *_self;
    unsigned int _amount;
  };

  SelfDamage::SelfDamage(IHealthTarget *self, unsigned int amount) : _self(self), _amount(amount){}
  SelfDamage::~SelfDamage() {}

  bool SelfDamage::perform([[maybe_unused]] unsigned int &damage) {
    _self->take_damage(_amount);
    return !_self->is_dead();
  }

}
