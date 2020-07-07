#pragma once

#include <map>
#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  enum EnergyType {
    GRASS,
    FIRE,
    WATER,
    LIGHTNING,
    PSYCHIC,
    FIGHTING,
    DARKNESS,
    METAL,
    FAIRY,
    DRAGON,
    COLORLESS,
  };

  class EnergyAmount {
  public:
    EnergyAmount();
    EnergyAmount(const std::vector<EnergyType> &energy);

    // Mutators
    void operator+=(const EnergyType &energy);
    void operator+=(const EnergyAmount &energy);
    void operator-=(const EnergyType &energy);
    void operator-=(const EnergyAmount &energy);

    // Accessors
    unsigned int operator[](EnergyType energy);
    bool contains(EnergyType energy);
    bool contains(EnergyAmount energy);
    bool covers_cost(EnergyAmount cost);
    operator std::string() const;

  private:
    std::map<EnergyType, unsigned int> _energy;
  };

}
