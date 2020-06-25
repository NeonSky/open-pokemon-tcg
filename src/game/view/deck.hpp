#pragma once

#include "card.hpp"
#include "../model/deck.hpp"

#include <vector>

namespace open_pokemon_tcg::game::view {

  struct Deck {
    std::string name;
    std::vector<Card> cards;

    Deck(const game::model::Deck &model) {
      name = model.name;
      for (auto &card : model.cards)
        cards.push_back(Card(engine::geometry::Transform(), *card));
    }
  };

}
