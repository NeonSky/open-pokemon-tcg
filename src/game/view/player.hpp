#pragma once

#include "bench.hpp"
#include "card.hpp"
#include "pokemon_card.hpp"
#include "deck_pile.hpp"
#include "discard_pile.hpp"
#include "hand.hpp"
#include "playmat.hpp"
#include "prize_card_pool.hpp"

#include "../model/player.hpp"

namespace open_pokemon_tcg::game::view {

  class Player {
  public:
    Player(const model::Player &model, const IPlaymat &playmat, IPlaymat::Side playmat_side);
    ~Player();

    DeckPile *deck_pile;
    DiscardPile *discard_pile;
    Hand *hand;
    PokemonCard *active_pokemon;
    Card *supporter_card;
    Card *stadium_card;
    Bench *bench;
    PrizeCardPool *prize_card_pool;

    // Mutators
    void update();
    void render(const glm::mat4 &view_projection);

  private:
    static const int start_hand = 7;

    const model::Player &_model;
    engine::graphics::Shader *_shader; // TODO: remove

    const open_pokemon_tcg::game::view::IPlaymat *playmat;
    IPlaymat::Side playmat_side;
    void on_update_active(model::PokemonCard* card);
    //void on_update_suppoter(model::TrainerCard* card);
    //void on_update_stadium(model::TrainerCard* card);

  };
}
