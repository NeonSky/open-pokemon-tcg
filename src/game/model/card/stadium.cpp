#include "stadium.hpp"

using namespace open_pokemon_tcg::game::model;

StadiumCard::StadiumCard(CardId id, CardName name, std::unique_ptr<ICardEffect> effect)
  : _id(id),
    _name(name),
    _effect(effect.release()){}

StadiumCard::~StadiumCard() {}

CardId StadiumCard::id() const {
  return _id;
}
CardName StadiumCard::name() const {
  return _name;
}

std::string StadiumCard::to_string() const {
  return name() + " (" + id() + ")";
}
