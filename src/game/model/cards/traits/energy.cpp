#include "energy.hpp"

#include "../../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model::cards::traits;


EnergyAmount::EnergyAmount() {}
EnergyAmount::EnergyAmount(const std::vector<EnergyType> &energy) {
  for (auto &e : energy)
    _energy[e]++;
}

// Mutators
void EnergyAmount::operator+=(const EnergyType &energy) {
  _energy[energy]++;
}

void EnergyAmount::operator+=(const EnergyAmount &energy) {
  for (auto &e : energy._energy)
    _energy[e.first] += e.second;
}

void EnergyAmount::operator-=(const EnergyType &energy) {
  if (!contains(energy))
    LOG_ERROR("Negative amount of energy is not allowed.");

  _energy[energy]--;
}

void EnergyAmount::operator-=(const EnergyAmount &energy) {
  if (!contains(energy))
    LOG_ERROR("Negative amount of energy is not allowed.");

  for (auto &e : energy._energy)
    _energy[e.first] -= e.second;
}

// Accessors
unsigned int EnergyAmount::operator[](EnergyType energy) {
  return _energy[energy];
}

bool EnergyAmount::contains(EnergyType energy) {
  return _energy[energy] > 0;
}

bool EnergyAmount::contains(EnergyAmount energy) {
  for (auto &e : energy._energy)
    if (_energy[e.first] < e.second)
      return false;
  return true;
}
