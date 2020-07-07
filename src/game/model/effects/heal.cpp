#include "heal.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

Heal::Heal(CardEffectTarget scope, unsigned int heal_amount)
  : _scope(scope),
    _heal_amount(heal_amount) {}

Heal::~Heal() = default;

void Heal::activate(Player &self, [[maybe_unused]] Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) {
  const PokemonCard* pokemon_target = dynamic_cast<const PokemonCard*>(&targets[0].get());
  if (pokemon_target == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  self.heal_pokemon(*pokemon_target, _heal_amount);
}

std::vector<CardEffectTarget> Heal::required_targets() const {
  return { _scope };
}
