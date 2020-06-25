#pragma once

#include "bench.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "deck_pile.hpp"
#include "discard_pile.hpp"
#include "hand.hpp"
#include "playmat.hpp"
#include "prize_card_pool.hpp"

#include "../model/player.hpp"

namespace open_pokemon_tcg::game::view {

  class Player {
  public:
    Player(const model::Player &player, const open_pokemon_tcg::game::view::IPlaymat &playmat, open_pokemon_tcg::game::view::IPlaymat::Side playmat_side);
    ~Player();

    DeckPile *deck_pile;
    DiscardPile *discard_pile;
    Hand *hand;
    Card *active_pokemon;
    Card *supporter_card;
    Card *stadium_card;
    Bench *bench;
    PrizeCardPool *prize_card_pool;

    // Mutators
    void update();
    void render(const glm::mat4 &view_projection);

    void mill();
    void place_active_pokemon(Card *card);
    void place_bench_pokemon(Card *card);

  private:
    static const int start_hand = 7;

    const model::Player &_model;
    engine::graphics::Shader *_shader; // TODO: remove

    Deck deck;
    const open_pokemon_tcg::game::view::IPlaymat *playmat;
    open_pokemon_tcg::game::view::IPlaymat::Side playmat_side;
  };
}
