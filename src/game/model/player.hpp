#pragma once

#include "ioperate_player_side.hpp"
#include "playmat.hpp"
#include "hand.hpp"
#include "card/pokemon.hpp"

#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class Player : public IOperatePlayerSide {
  public:
    Player(std::unique_ptr<Deck>& deck, Playmat& playmat, std::string name);
    ~Player();

    // Mutators
    void shuffle_deck_pile();
    void draw(unsigned int amount = 1) override;
    void mill(unsigned int amount = 1);
    void discard(ICard& card);
    void place_on_active_slot_from_hand(ICard& card);
    void place_on_bench_from_hand(ICard& card);
    void place_on_bench_from_hand(ICard& card, unsigned int slot_index);

    void place_active_pokemon(unsigned int hand_index) override;
    void activate_trainer_card(unsigned int hand_index);
    void take_prize_card();
    void bench_pokemon_from_hand(ICard& card);

    void on_win(std::function<void ()> callback);
    void on_lose(std::function<void ()> callback);

    // Accessors
    void on_update_active(std::function<void (PokemonCard* card)> callback) const;
    //void on_update_supporter(std::function<void (TrainerCard* card)> callback);
    //void on_update_stadium(std::function<void (TrainerCard* card)> callback);

    std::string name() const;
    const Deck& deck() const override;
    const Playmat& playmat() const;
    const Hand& hand() const;

  private:
    mutable engine::event::CallbackList<void ()> _on_win;
    mutable engine::event::CallbackList<void ()> _on_lose;
    mutable engine::event::CallbackList<void (PokemonCard* card)> _on_update_active;
    //engine::event::CallbackList<void (TrainerCard* card)> _on_update_supporter;
    //engine::event::CallbackList<void (TrainerCard* card)> _on_update_stadium;

    std::unique_ptr<Deck> _deck;
    Playmat _playmat;
    std::string _name;
    Hand _hand;

  };

}
