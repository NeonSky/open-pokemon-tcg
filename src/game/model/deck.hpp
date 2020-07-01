#pragma once

#include "card/card.hpp"

#include <memory>
#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  struct Deck {
    std::string name;
    std::vector<std::unique_ptr<ICard>> cards;
  };

}
