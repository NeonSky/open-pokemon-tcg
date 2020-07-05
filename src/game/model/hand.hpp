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
    ICard& take(int index);

    // Accessors
    void listen_on_add(std::function<void (ICard& card)> callback) const;
    void listen_on_remove(std::function<void (int index)> callback) const;

    const std::vector<std::reference_wrapper<ICard>>& cards() const;
    unsigned int size() const;
    unsigned int find(const ICard &card) const;

  private:
    mutable engine::event::CallbackList<void (ICard& card)> _on_add;
    mutable engine::event::CallbackList<void (int index)> _on_remove;

    std::vector<std::reference_wrapper<ICard>> _cards;
  };
}
