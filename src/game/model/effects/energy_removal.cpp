#include "energy_removal.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

EnergyRemoval::EnergyRemoval() {
  _pokemon_target = nullptr;
  _energy_target = nullptr;
}

EnergyRemoval::~EnergyRemoval() = default;

// TODO: Move into activate() and remove this function
void EnergyRemoval::set_targets(std::vector<std::reference_wrapper<const ICard>> targets) {
  const PokemonCard* pokemon = dynamic_cast<const PokemonCard*>(&targets[0].get());
  if (pokemon == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  const BasicEnergy* energy = dynamic_cast<const BasicEnergy*>(&targets[1].get());
  if (energy == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  _pokemon_target = pokemon;
  _energy_target = energy;
}

void EnergyRemoval::activate([[maybe_unused]] Player &self, Player &opponent) {
  opponent.remove_energy_from_active(*_energy_target);
}

std::vector<CardEffectTarget> EnergyRemoval::required_targets() const {
  return {
      CardEffectTarget::ENEMY_POKEMON,
      CardEffectTarget::ENERGY_FROM_PREVIOUS_POKEMON,
  };
}

bool EnergyRemoval::can_activate([[maybe_unused]] Player &self, [[maybe_unused]] Player &opponent) const {
  return _pokemon_target != nullptr && _energy_target != nullptr;
}
