#pragma once

#include "card.hpp"

#include "../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class DeckPile {
  public:
    DeckPile();
    DeckPile(std::vector<ICard*> cards);
    ~DeckPile();

    // Mutators
    ICard& pop();
    void listen_on_pop(std::function<void ()> callback);
    void shuffle();

    // Accessors
    unsigned int size() const;
    const std::vector<ICard*>& cards() const;

  private:
    std::vector<ICard*> _cards;

    engine::event::CallbackList<void ()> _on_pop;
  };
}
