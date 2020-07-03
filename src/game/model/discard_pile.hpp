#pragma once

#include "card/card.hpp"

#include "../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class DiscardPile {
  public:
    DiscardPile();
    ~DiscardPile();

    // Mutators
    void push(ICard& card);

    // Accessors
    void on_push(std::function<void (ICard&)> callback) const;
    unsigned int size() const;
    const std::vector<std::reference_wrapper<ICard>> cards() const;

  private:
    mutable engine::event::CallbackList<void (ICard&)> _on_push;

    std::vector<std::reference_wrapper<ICard>> _cards;
  };
}
