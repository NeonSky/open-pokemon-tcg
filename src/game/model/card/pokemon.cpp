#include "pokemon.hpp"

using namespace open_pokemon_tcg::game::model;

PokemonCard::PokemonCard(PokemonCardData data) : _data(data) {}
PokemonCard::~PokemonCard() {}

void PokemonCard::take_damage(unsigned int amount) {
  _data.hp = std::max(0, _data.hp - (int)amount);
}

void PokemonCard::attack(unsigned int attack_index, IHealthTarget &opponent) {
  _data.attacks[attack_index].perform(opponent);
}

unsigned int PokemonCard::max_hp() const { return _data.hp; }
unsigned int PokemonCard::hp() const { return _hp; }

CardId PokemonCard::id() const {
  return _data.id;
}
CardName PokemonCard::name() const {
  return _data.name;
}

std::string PokemonCard::to_string() const {
  return name() + " (" + id() + ") with " + std::to_string(_data.hp) + " hp";
}

const PokemonCardData& PokemonCard::data() const {
  return _data;
}
