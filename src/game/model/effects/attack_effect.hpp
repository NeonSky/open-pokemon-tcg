#pragma once

#include "effect_target.hpp"

namespace open_pokemon_tcg::game::model {

  class IAttackEffect {
  public:
    IAttackEffect() {}
    virtual ~IAttackEffect() = 0;

    // Mutators
    virtual bool perform(unsigned int &damage) = 0; // returns false if the attack damage should be canceled
  };

}
