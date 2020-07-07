#pragma once

#include "energy.hpp"
#include "../../effects/attack_effect.hpp"

#include <memory>
#include <string>

namespace open_pokemon_tcg::game::model {

  class PokemonCard;

  class Attack {
  public:
    Attack(std::string name, unsigned int damage, EnergyAmount cost);
    Attack(std::string name, unsigned int damage, EnergyAmount cost, std::unique_ptr<IAttackEffect> effect);

    // Mutators
    IAttackEffect* effect() const;

    // Accessors
    std::string name() const;
    unsigned int damage() const;
    EnergyAmount cost() const;

  private:
    std::string _name;
    unsigned int _damage;
    EnergyAmount _cost;
    std::unique_ptr<IAttackEffect> _effect;
  };

}
