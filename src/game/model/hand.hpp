#pragma once

#include "card/card.hpp"

#include "../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class Hand {
  public:
    Hand();
    Hand(std::vector<std::reference_wrapper<ICard>> cards);
    ~Hand();

    // Mutators
    void add(ICard& card);
    void remove(int index);
    void listen_on_add(std::function<void (ICard& card)> callback);
    void listen_on_remove(std::function<void (int index)> callback);

    // Accessors
    unsigned int size() const;
    const std::vector<std::reference_wrapper<ICard>>& cards() const;
    unsigned int find(ICard &card) const;

  private:
    std::vector<std::reference_wrapper<ICard>> _cards;

    engine::event::CallbackList<void (ICard& card)> _on_add;
    engine::event::CallbackList<void (int index)> _on_remove;
  };
}
