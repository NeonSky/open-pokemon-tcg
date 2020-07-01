#include "supporter.hpp"

using namespace open_pokemon_tcg::game::model;

SupporterCard::SupporterCard(CardId id, CardName name, std::unique_ptr<ICardEffect> effect)
  : _id(id),
    _name(name),
    _effect(effect.release()){}

SupporterCard::~SupporterCard() {}

CardId SupporterCard::id() const {
  return _id;
}
CardName SupporterCard::name() const {
  return _name;
}

CardType SupporterCard::card_type() const {
  return CardType::SUPPORTER;
}

std::string SupporterCard::to_string() const {
  return name() + " (" + id() + ")";
}
