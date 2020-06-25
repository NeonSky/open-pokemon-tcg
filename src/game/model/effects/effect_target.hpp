#pragma once

namespace open_pokemon_tcg::game::model::effects {

  class IEffectTarget {};

  class IHealthTarget : public IEffectTarget {
  public:
    // Mutators
    virtual void take_damage(unsigned int amount) = 0;

    // Accessors
    virtual unsigned int max_hp() const = 0;
    virtual unsigned int hp() const = 0;
    virtual bool is_dead() const { return hp() == 0; }
  };

}
