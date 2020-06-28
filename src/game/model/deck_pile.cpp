#include "deck_pile.hpp"

using namespace open_pokemon_tcg::game::model;

DeckPile::DeckPile() = default;
DeckPile::DeckPile(std::vector<ICard*> cards) : _cards(cards) {}
DeckPile::~DeckPile() = default;

ICard& DeckPile::pop() {
  ICard &card = *_cards.back();
  _cards.pop_back();
  _on_pop();
  return card;
}

void DeckPile::listen_on_pop(std::function<void ()> callback) {
  _on_pop.append(callback);
}

void DeckPile::shuffle() {
  random_shuffle(_cards.begin(), _cards.end());
}

unsigned int DeckPile::size() const {return _cards.size(); }
const std::vector<ICard*>& DeckPile::cards() const { return _cards; }
