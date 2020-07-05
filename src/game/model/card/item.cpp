#include "item.hpp"

using namespace open_pokemon_tcg::game::model;

ItemCard::ItemCard(CardId id, CardName name, std::unique_ptr<ICardEffect> effect)
  : _id(id),
    _name(name),
    _effect(std::move(effect)) {}

ItemCard::~ItemCard() {}

CardId ItemCard::id() const {
  return _id;
}
CardName ItemCard::name() const {
  return _name;
}

std::string ItemCard::to_string() const {
  return name() + " (" + id() + ")";
}

ICardEffect& ItemCard::effect() {
  return *_effect;
}
