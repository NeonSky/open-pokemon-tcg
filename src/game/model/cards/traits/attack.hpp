#pragma once

#include "energy.hpp"
#include "../../effects/attack_effect.hpp"

#include <string>

namespace open_pokemon_tcg::game::model::cards::traits {

  class Attack {
  public:
    Attack();
    Attack(std::string name, unsigned int damage, EnergyAmount cost);
    Attack(std::string name, unsigned int damage, EnergyAmount cost, effects::IAttackEffect &effect);
    ~Attack();

    // Mutators
    void perform(effects::IHealthTarget &target);

    // Accessors
    std::string name() const;
    unsigned int damage() const;
    EnergyAmount cost() const;

  private:
    std::string _name;
    unsigned int _damage;
    EnergyAmount _cost;
    effects::IAttackEffect *_effect;
  };

}
