#include "trainer_card.hpp"

using namespace open_pokemon_tcg::game::model;
using namespace open_pokemon_tcg::game::model::cards;

TrainerCard::TrainerCard(TrainerCardData data) : _data(data) {}
TrainerCard::~TrainerCard() {}

effects::ICardEffect& TrainerCard::effect() {
  return *_data.effect;
}

std::string TrainerCard::debug() const {
  return name() + " (" + id() + ")";
}

CardId TrainerCard::id() const {
  return _data.id;
}
CardName TrainerCard::name() const {
  return _data.name;
}

const TrainerCardData& TrainerCard::data() const {
  return _data;
}
