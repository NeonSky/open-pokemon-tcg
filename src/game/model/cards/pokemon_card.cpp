#include "pokemon_card.hpp"

using namespace open_pokemon_tcg::game::model;
using namespace open_pokemon_tcg::game::model::cards;

PokemonCard::PokemonCard(PokemonCardData data) : _data(data) {}
PokemonCard::~PokemonCard() {}

void PokemonCard::take_damage(unsigned int amount) {
  _data.hp = std::max(0, _data.hp - (int)amount);
}

void PokemonCard::attack(unsigned int attack_index, effects::IHealthTarget &opponent) {
  _data.attacks[attack_index].perform(opponent);
}

unsigned int PokemonCard::max_hp() const { return _data.hp; }
unsigned int PokemonCard::hp() const { return _hp; }

std::string PokemonCard::debug() {
  return name() + " (" + id() + ") with " + std::to_string(_data.hp) + " hp";
}

CardId PokemonCard::id() {
  return _data.id;
}
CardName PokemonCard::name() {
  return _data.name;
}

const PokemonCardData& PokemonCard::data() const {
  return _data;
}
