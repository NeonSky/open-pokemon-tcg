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

    void listen_on_place(std::function<void (ICard &card, unsigned int index)> callback);
    void listen_on_take(std::function<void (unsigned int index)> callback);

    // Accessors
    const std::array<ICard*, 5> cards() const;

  private:
    std::array<ICard*, 5> _cards;

    engine::event::CallbackList<void (ICard&, unsigned int)> _on_place;
    engine::event::CallbackList<void (unsigned int)> _on_take;
  };
}
