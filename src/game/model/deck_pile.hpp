#pragma once

#include "deck.hpp"
#include "card/card.hpp"

#include "../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class DeckPile {
  public:
    DeckPile();
    DeckPile(Deck& deck);
    ~DeckPile();

    // Mutators
    ICard& pop();
    void shuffle();

    // Accessors
    void on_pop(std::function<void ()> callback) const;
    unsigned int size() const;
    const std::vector<std::reference_wrapper<ICard>>& cards() const;

  private:
    mutable engine::event::CallbackList<void ()> _on_pop;

    std::vector<std::reference_wrapper<ICard>> _cards;
  };
}
