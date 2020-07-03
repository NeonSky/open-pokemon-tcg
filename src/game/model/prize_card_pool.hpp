#pragma once

#include "card/card.hpp"

#include "../../engine/event/event.hpp"

#include <array>
#include <functional>

namespace open_pokemon_tcg::game::model {

  class PrizeCardPool {
  public:
    PrizeCardPool(std::array<std::reference_wrapper<ICard>, 6> prize_cards);
    ~PrizeCardPool();

    // Mutators
    ICard& take_any();
    ICard& take(unsigned int index);

    // Accessors
    void on_take(std::function<void (unsigned int index)> callback) const;
    bool empty() const;
    const std::array<ICard*, 6> cards() const;

  private:
    mutable engine::event::CallbackList<void (unsigned int)> _on_take;

    std::array<ICard*, 6> _cards;
  };
}
