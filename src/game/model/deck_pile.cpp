#include "deck_pile.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

DeckPile::DeckPile() = default;

DeckPile::DeckPile(Deck& deck) {
  for (std::unique_ptr<ICard>& c : deck.cards)
    _cards.push_back(std::ref(*c));
}

DeckPile::~DeckPile() = default;

ICard& DeckPile::pop() {
  ICard &card = _cards.back().get();
  _cards.pop_back();
  _on_pop();
  return card;
}

void DeckPile::on_pop(std::function<void ()> callback) const {
  _on_pop.append(callback);
}

void DeckPile::shuffle() {
  random_shuffle(_cards.begin(), _cards.end());
}

unsigned int DeckPile::size() const {return _cards.size(); }
const std::vector<std::reference_wrapper<ICard>>& DeckPile::cards() const { return _cards; }
