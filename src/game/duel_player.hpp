#pragma once

#include "playmat.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "deck_pile.hpp"
#include "discard_pile.hpp"
#include "prize_card_pool.hpp"
#include "hand.hpp"

namespace open_pokemon_tcg {

  class DuelPlayer {
  public:
    DuelPlayer(Deck deck, const IPlaymat &playmat, IPlaymat::Side playmat_side);
    ~DuelPlayer();

    DeckPile *deck_pile;
    DiscardPile *discard_pile;
    Hand *hand;
    Card *active_pokemon;
    Card *suporter_card;
    Card *stadium_card;
    // Bench bench;
    PrizeCardPool *prize_card_pool;

    // Mutators
    void update();
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

    void mill();

  private:
    static const int start_hand = 7;

    Deck deck;
    const IPlaymat *playmat;
    IPlaymat::Side playmat_side;
  };
}
