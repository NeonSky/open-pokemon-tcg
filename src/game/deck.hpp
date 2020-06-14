#pragma once

#include "card.hpp"

#include <vector>

namespace open_pokemon_tcg {

  struct Deck {
    std::vector<Card> cards;
  };

}
