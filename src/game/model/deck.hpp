#pragma once

#include "card.hpp"

#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  struct Deck {
    std::string name;
    std::vector<ICard*> cards;
  };

}
