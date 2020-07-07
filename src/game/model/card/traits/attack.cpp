#include "attack.hpp"

using namespace open_pokemon_tcg::game::model;

Attack::Attack(std::string name, unsigned int damage, EnergyAmount cost)
  : _name(name),
    _damage(damage),
    _cost(cost) {}

Attack::Attack(std::string name, unsigned int damage, EnergyAmount cost, std::unique_ptr<IAttackEffect> effect)
  : _name(name),
    _damage(damage),
    _cost(cost),
    _effect(std::move(effect)) {}

// Mutators
IAttackEffect* Attack::effect() const {
  return _effect.get();
}

// Accessors
std::string Attack::name() const { return _name; }
unsigned int Attack::damage() const { return _damage; }
EnergyAmount Attack::cost() const { return _cost; }
