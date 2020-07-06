#include "energy_removal.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

EnergyRemoval::EnergyRemoval(CardEffectTarget pokemon_scope)
  : _pokemon_scope(pokemon_scope) {}

EnergyRemoval::~EnergyRemoval() = default;

void EnergyRemoval::activate([[maybe_unused]] Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) {
  const PokemonCard* pokemon_target = dynamic_cast<const PokemonCard*>(&targets[0].get());
  if (pokemon_target == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  const BasicEnergy* energy_target = dynamic_cast<const BasicEnergy*>(&targets[1].get());
  if (energy_target == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  opponent.detach_energy_from(*pokemon_target, *energy_target);
}

std::vector<CardEffectTarget> EnergyRemoval::required_targets() const {
  return {
      _pokemon_scope,
      CardEffectTarget::ENERGY_FROM_PREVIOUS_POKEMON,
  };
}
