#include "trainer_card.hpp"

using namespace open_pokemon_tcg::game::model;

TrainerCard::TrainerCard(TrainerCardData data) : _data(data) {}
TrainerCard::~TrainerCard() {}

ICardEffect& TrainerCard::effect() {
  return *_data.effect;
}

std::string TrainerCard::to_string() const {
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
