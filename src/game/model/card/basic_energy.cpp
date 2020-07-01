#include "basic_energy.hpp"

using namespace open_pokemon_tcg::game::model;

BasicEnergy::BasicEnergy(std::string id, std::string name, EnergyType energy)
  : _id(id),
    _name(name),
    _energy_type(energy) {}

BasicEnergy::~BasicEnergy() {}

CardId BasicEnergy::id() const {
  return _id;
}

CardName BasicEnergy::name() const {
  return _name;
}

CardType BasicEnergy::card_type() const {
  return CardType::ENERGY;
}

std::string BasicEnergy::to_string() const {
  return name() + " (" + id() + ")";
}

bool BasicEnergy::is_basic() const {
  return true;
}

EnergyType BasicEnergy::energy_type() const {
  return _energy_type;
}
