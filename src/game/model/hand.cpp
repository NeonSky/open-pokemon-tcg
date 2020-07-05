#include "hand.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

Hand::Hand() = default;
Hand::Hand(std::vector<std::reference_wrapper<ICard>> cards) : _cards(cards) {}
Hand::~Hand() = default;

void Hand::add(ICard& card) {
  _cards.push_back(card);
  _on_add(card);
}

void Hand::remove(int index) {
  _cards.erase(_cards.begin() + index);
  _on_remove(index);
}

ICard& Hand::take(int index) {
  ICard& card = _cards[index];
  remove(index);
  return card;
}

void Hand::listen_on_add(std::function<void (ICard& card)> callback) const {
  _on_add.append(callback);
}

void Hand::listen_on_remove(std::function<void (int index)> callback) const {
  _on_remove.append(callback);
}

unsigned int Hand::size() const {return _cards.size(); }

const std::vector<std::reference_wrapper<ICard>>& Hand::cards() const { return _cards; }

unsigned int Hand::find(const ICard &card) const {
  for (unsigned int i = 0; i < _cards.size(); i++)
    if (&_cards[i].get() == &card)
      return i;

  LOG_ERROR("Could not find card in hand: " + card.to_string());
}
