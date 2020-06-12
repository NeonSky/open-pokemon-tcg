#pragma once

#include "playmat.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "deck_pile.hpp"
#include "discard_pile.hpp"
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
    // PrizeCardPool prize_cards;

    // Mutators
    void update();
    void render(const glm::mat4 &view_projection_matrix, Shader *shader);

    void mill();

  private:
    Deck deck;
    const IPlaymat *playmat;
    IPlaymat::Side playmat_side;
  };
}
