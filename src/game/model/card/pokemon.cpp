#include "pokemon.hpp"

#include "../../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

PokemonCard::PokemonCard(PokemonCardData data)
  : _data(data) {

  _hp = _data.hp;
}

PokemonCard::~PokemonCard() = default;

void PokemonCard::take_damage(unsigned int amount) {
  _data.hp = std::max(0, _data.hp - (int)amount);
}

void PokemonCard::attack(unsigned int attack_index, IHealthTarget &opponent) {
  _data.attacks[attack_index].perform(opponent);
}

void PokemonCard::attach_energy(BasicEnergy energy_card) {
  _attached_energy.push_back(energy_card);
  _on_energy_attached(energy_card);
}

void PokemonCard::detach_energy(unsigned int index) {
  _attached_energy.erase(_attached_energy.begin() + index);
  _on_energy_detached(index);
}

void PokemonCard::detach_energy(const BasicEnergy& energy_card) {
  for (unsigned int i = 0; i < _attached_energy.size(); i++) {
    if (&energy_card == &_attached_energy[i]) {
      detach_energy(i);
      return;
    }
  }

  LOG_ERROR("Did not find energy card to detach.");
}

void PokemonCard::on_energy_attached(std::function<void (const BasicEnergy& energy_card)> callback) const {
  _on_energy_attached.append(callback);
}

void PokemonCard::on_energy_detached(std::function<void (unsigned int index)> callback) const {
  _on_energy_detached.append(callback);
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

const std::vector<BasicEnergy>& PokemonCard::attached_energy() const {
  return _attached_energy;
}
