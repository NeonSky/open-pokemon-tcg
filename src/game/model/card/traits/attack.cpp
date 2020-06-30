#include "attack.hpp"

using namespace open_pokemon_tcg::game::model;

Attack::Attack() {} // FIXME: remove
Attack::Attack(std::string name, unsigned int damage, EnergyAmount cost)
  : _name(name), _damage(damage), _cost(cost), _effect(nullptr) {}

Attack::Attack(std::string name, unsigned int damage, EnergyAmount cost, IAttackEffect &effect)
  : _name(name), _damage(damage), _cost(cost), _effect(&effect) {}

Attack::~Attack() {}

// Mutators
void Attack::perform(IHealthTarget &target) {
  // NOTE: check cost requirement

  unsigned int modified_damage = _damage; // NOTE: Important to copy so the original damage is not modified by effects.
  if (_effect->perform(modified_damage))
    target.take_damage(modified_damage);
}

// Accessors
std::string Attack::name() const { return _name; }
unsigned int Attack::damage() const { return _damage; }
EnergyAmount Attack::cost() const { return _cost; }
