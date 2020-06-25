#pragma once

#include "card.hpp"
#include "pokemon_card.hpp"

#include <array>
#include <vector>

namespace open_pokemon_tcg::game::model {

  struct Playmat {
    std::vector<ICard*> deck_pile;
    std::vector<ICard*> discard_pile;
    PokemonCard* active_pokemon = nullptr;
    ICard* supporter_card = nullptr;
    ICard* stadium_card = nullptr;
    std::array<ICard*, 5> bench_cards;
    std::array<ICard*, 6> prize_cards;
  };

}
