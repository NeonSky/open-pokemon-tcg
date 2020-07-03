#pragma once

#include "card/card.hpp"

#include "../../engine/event/event.hpp"

#include <array>
#include <functional>

namespace open_pokemon_tcg::game::model {

  class Bench {
  public:
    Bench();
    ~Bench();

    // Mutators
    void place(ICard &card);
    void place(ICard &card, unsigned int index);
    ICard& take(unsigned int index);

    // Accessors
    void on_place(std::function<void (ICard &card, unsigned int index)> callback) const;
    void on_take(std::function<void (unsigned int index)> callback) const;
    const std::array<ICard*, 5> cards() const;

  private:
    mutable engine::event::CallbackList<void (ICard&, unsigned int)> _on_place;
    mutable engine::event::CallbackList<void (unsigned int)> _on_take;

    std::array<ICard*, 5> _cards;
  };
}
