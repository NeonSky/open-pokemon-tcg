#pragma once

#include "pokemon_trainer.hpp"
#include "playmat.hpp"
#include "game_master.hpp"
#include "hand.hpp"
#include "card/pokemon.hpp"

#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  // TODO: Switch these names to PokemonTrainer and IPlayer
  class Player : public IPokemonTrainer {
  public:
    Player(IGameMaster& gm, std::unique_ptr<Deck>& deck, Playmat& playmat, std::string name);
    ~Player();

    // Mutators
    void shuffle_deck_pile();
    void draw(unsigned int amount = 1) override;
    void mill(unsigned int amount = 1);
    void place_active_pokemon(unsigned int hand_index) override;
    void activate_trainer_card(unsigned int hand_index);
    void active_pokemon_from_hand(ICard& card);
    void take_prize_card();
    void bench_pokemon_from_hand(ICard& card);

    void on_update_active(std::function<void (PokemonCard* card)> callback);
    //void on_update_supporter(std::function<void (TrainerCard* card)> callback);
    //void on_update_stadium(std::function<void (TrainerCard* card)> callback);

    // Accessors
    bool lost() const;
    bool won() const; // TODO: Maybe use event instead.
    std::string name() const;
    const Deck& deck() const override;
    Playmat& playmat();
    Hand& hand();

  private:
    IGameMaster &_gm;
    std::unique_ptr<Deck> _deck;
    Playmat _playmat;
    std::string _name;
    Hand _hand;
    bool _lost;
    bool _won;

    engine::event::CallbackList<void (PokemonCard* card)> _on_update_active;
    //engine::event::CallbackList<void (TrainerCard* card)> _on_update_supporter;
    //engine::event::CallbackList<void (TrainerCard* card)> _on_update_stadium;
  };

}
