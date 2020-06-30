#pragma once

#include "card.hpp"
#include "card/pokemon_card.hpp"
#include "deck_pile.hpp"
#include "discard_pile.hpp"
#include "bench.hpp"
#include "prize_card_pool.hpp"

#include <array>
#include <vector>

namespace open_pokemon_tcg::game::model {

  struct Playmat {
    DeckPile *deck_pile = nullptr;
    DiscardPile *discard_pile = nullptr;
    PokemonCard* active_pokemon = nullptr;
    ICard* supporter_card = nullptr;
    ICard* stadium_card = nullptr;
    Bench *bench = nullptr;
    PrizeCardPool *prize_card_pool = nullptr;
  };

}
