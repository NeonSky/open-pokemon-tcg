#include "discard_pile.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

DiscardPile::DiscardPile() = default;
DiscardPile::~DiscardPile() = default;

void DiscardPile::push(ICard& card) {
  _cards.push_back(card);
  _on_push(card);
}

void DiscardPile::listen_on_push(std::function<void (ICard&)> callback) {
  _on_push.append(callback);
}

const std::vector<std::reference_wrapper<ICard>> DiscardPile::cards() const {
  return _cards;
}

unsigned int DiscardPile::size() const {return _cards.size(); }
