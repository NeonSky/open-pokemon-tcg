#pragma once

#include "pokemon_trainer.hpp"
#include "playmat.hpp"
#include "game_master.hpp"
#include "hand.hpp"

#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  // TODO: Switch these names to PokemonTrainer and IPlayer
  class Player : public IPokemonTrainer {
  public:
    Player(IGameMaster &gm, Deck deck, Playmat &playmat, std::string name);
    ~Player();

    // Mutators
    void shuffle_deck_pile();
    void draw(unsigned int amount = 1) override;
    void mill(unsigned int amount = 1);
    void place_active_pokemon(unsigned int hand_index) override;
    void activate_trainer_card(unsigned int hand_index);
    void take_prize_card();
    void bench_pokemon_from_hand(ICard& card);

    // Accessors
    bool lost() const;
    bool won() const; // TODO: Maybe use event instead.
    std::string name() const;
    Playmat& playmat();
    const Deck& deck() const override;
    Hand& hand();

  private:
    IGameMaster &_gm;
    Deck _deck;
    Playmat _playmat;
    std::string _name;
    Hand _hand;
    bool _lost;
    bool _won;
  };

}
