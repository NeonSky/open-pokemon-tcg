#pragma once

#include "pokemon_trainer.hpp"
#include "playmat.hpp"
#include "game_master.hpp"

#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  // TODO: Switch these names to PokemonTrainer and IPlayer
  class Player : public IPokemonTrainer {
  public:
    Player(IGameMaster &gm, Deck deck, Playmat &playmat, std::string name);
    ~Player();

    // Mutators
    void draw(unsigned int amount = 1) override;
    void place_active_pokemon(unsigned int hand_index) override;
    void activate_trainer_card(unsigned int hand_index);

    // Accessors
    bool lost() const;
    std::string name() const;
    const Playmat& playmat() const;
    const Deck& deck() const override;
    const std::vector<ICard*>& hand() const;

  private:
    IGameMaster &_gm;
    Deck _deck;
    Playmat _playmat;
    std::string _name;
    std::vector<ICard*> _hand;
    bool _lost;
  };

}
