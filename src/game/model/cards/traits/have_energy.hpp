#pragma once

#include "../../energy.hpp"

namespace open_pokemon_tcg::game::model::cards::traits {

  class IHaveEnergy {
  public:
    // Mutators
    void attach_energy(EnergyType energy);
    void attach_energy(EnergyAmount energy);
    void remove_energy(EnergyType energy);
    void remove_energy(EnergyAmount energy);

    // Accessors
    unsigned int energy_count() const;
    unsigned int energy_count(EnergyType energy) const;
    bool has_energy(EnergyType energy) const;
    bool has_energy(EnergyAmount energy) const;
  };

}
