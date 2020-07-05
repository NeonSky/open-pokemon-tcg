#include "bench.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

Bench::Bench()
  : _cards{} {}

Bench::~Bench() = default;

void Bench::place(PokemonCard &card) {
  for (unsigned int i = 0; i < _cards.size(); i++) {
    if (_cards[i] == nullptr) {
      _cards[i] = &card;
      _on_place(card, i);
      return;
    }
  }

  LOG_ERROR("Did not find any empty spot on bench to place card.");
}

void Bench::place(PokemonCard &card, unsigned int index) {
  if (index >= _cards.size())
    LOG_ERROR("Index out of range.");

  if (_cards[index] != nullptr)
    LOG_ERROR("Bench slot " + std::to_string(index) + " already occupied.");

  _cards[index] = &card;
  _on_place(card, index);
}

PokemonCard& Bench::take(unsigned int index) {
  if (index >= _cards.size())
    LOG_ERROR("Index out of range.");

  PokemonCard *card = _cards[index];
  _cards[index] = nullptr;
  _on_take(index);

  return *card;
}

void Bench::on_place(std::function<void (PokemonCard &card, unsigned int index)> callback) const {
  _on_place.append(callback);
}

void Bench::on_take(std::function<void (unsigned int)> callback) const {
  _on_take.append(callback);
}

const std::array<PokemonCard*, 5> Bench::cards() const {
  return _cards;
}
