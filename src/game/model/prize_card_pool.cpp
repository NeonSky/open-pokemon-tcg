#include "prize_card_pool.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

PrizeCardPool::PrizeCardPool(std::array<std::reference_wrapper<ICard>, 6> prize_cards) {
  for (unsigned int i = 0; i < _cards.size(); i++)
    _cards[i] = &prize_cards[i].get();
}

PrizeCardPool::~PrizeCardPool() = default;


ICard& PrizeCardPool::take_any() {
  for (unsigned int i = 0; i < _cards.size(); i++)
    if (_cards[i] != nullptr)
      return take(i);

  LOG_ERROR("Could not find any prize card to take");
}

ICard& PrizeCardPool::take(unsigned int index) {
  if (index >= _cards.size())
    LOG_ERROR("Index out of range.");

  if (_cards[index] == nullptr)
    LOG_ERROR("This prize card has already been taken.");

  ICard *card = _cards[index];
  _cards[index] = nullptr;
  _on_take(index);

  return *card;
}

void PrizeCardPool::on_take(std::function<void (unsigned int)> callback) const {
  _on_take.append(callback);
}

bool PrizeCardPool::empty() const {
  return std::all_of(_cards.begin(), _cards.end(), [](ICard* c){ return c == nullptr; });
}

const std::array<ICard*, 6> PrizeCardPool::cards() const {
  return _cards;
}
