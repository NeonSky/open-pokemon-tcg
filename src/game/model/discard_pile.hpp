#pragma once

#include "card.hpp"

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
    void listen_on_push(std::function<void (ICard&)> callback);

    // Accessors
    unsigned int size() const;
    const std::vector<std::reference_wrapper<ICard>> cards() const;

  private:
    std::vector<std::reference_wrapper<ICard>> _cards;

    engine::event::CallbackList<void (ICard&)> _on_push;
  };
}
