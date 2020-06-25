#include "energy_card.hpp"

using namespace open_pokemon_tcg::game::model;

EnergyCard::EnergyCard(std::string id, std::string name, EnergyType energy) : _id(id), _name(name), _energy(energy) {}
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
