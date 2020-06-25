#include "energy_card.hpp"

using namespace open_pokemon_tcg::game::model;
using namespace open_pokemon_tcg::game::model::cards;

EnergyCard::EnergyCard(std::string id, std::string name, traits::EnergyType energy) : _id(id), _name(name), _energy(energy) {}
EnergyCard::~EnergyCard() {}


std::string EnergyCard::debug() {
  return name() + " (" + id() + ")";
}

CardId EnergyCard::id() {
  return _id;
}

CardName EnergyCard::name() {
  return _name;
}
