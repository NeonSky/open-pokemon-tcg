#pragma once

#include "../pokemon_trainer.hpp"

namespace open_pokemon_tcg::game::model {

  class ICardEffect {
  public:
    // Mutators
    virtual void activate(IPokemonTrainer &self, IPokemonTrainer &opponent) = 0;

    // Accessors
    virtual bool can_activate(IPokemonTrainer &self, IPokemonTrainer &opponent) = 0;
  };

}
