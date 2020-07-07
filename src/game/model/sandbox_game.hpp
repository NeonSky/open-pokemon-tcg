#pragma once

#include "player.hpp"
#include "playmat.hpp"
#include "effects/card_effect.hpp"

#include <memory>
#include <string>
#include <array>

namespace open_pokemon_tcg::game::model {

  class SandboxGame {
  public:
    SandboxGame(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names);
    ~SandboxGame();

    friend class Game;

    // Mutators
    void place_on_active_slot_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card, unsigned int slot_index);
    void activate_trainer_card(const ICard& card, std::vector<std::reference_wrapper<const ICard>> targets);

    void attach_to_active_pokemon(const ICard& card);
    void attach_to_bench_pokemon(const ICard& card, unsigned int slot_index);
    void draw(unsigned int amount = 1);
    void mill(unsigned int amount = 1);

    void attack(unsigned int attack_index);
    void end_turn();

    // Accessors
    void on_game_over(std::function<void ()> callback) const;
    // void on_turn_ended(std::function<void ()> callback) const; // TODO

    unsigned int turn() const;
    const Player* winner() const;
    const Player& current_player() const;
    const Player& next_player() const;

  private:
    mutable engine::event::CallbackList<void ()> _on_game_over;

    const Player* _winner;
    std::array<std::unique_ptr<Player>, 2> _players;
    std::array<std::shared_ptr<Playmat>, 2> _playmats;
    unsigned int _current_player;
    unsigned int _turn;

    unsigned int _next_player() const;
  };

}
