#include "switch_active.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

SwitchActive::SwitchActive(CardEffectTarget scope)
  : _scope(scope) {}

SwitchActive::~SwitchActive() = default;

void SwitchActive::activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) {
  const PokemonCard* pokemon_target = dynamic_cast<const PokemonCard*>(&targets[0].get());
  if (pokemon_target == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  if (_scope == FRIENDLY_BENCH_POKEMON)
    self.switch_active(*pokemon_target);
  else if (_scope == ENEMY_BENCH_POKEMON)
    opponent.switch_active(*pokemon_target);
  else
    LOG_ERROR("Scope is wrong. Check construction of object.");
}

std::vector<CardEffectTarget> SwitchActive::required_targets() const {
  return { _scope };
}
