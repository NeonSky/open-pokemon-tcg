#pragma once

#include "player.hpp"
#include "playmat.hpp"

#include <memory>
#include <string>
#include <array>

namespace open_pokemon_tcg::game::model {

  class Game : public IGameMaster {
  public:
    Game(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names);
    ~Game();

    // Player controls
    void place_on_active_slot_from_hand(ICard& card);
    void place_on_bench_from_hand(ICard& card);
    void place_on_bench_slot_from_hand(ICard& card, unsigned int slot_index);
    void evolve_pokemon();
    void attack(unsigned int attack_index);
    void activate(ICardEffect& effect) override;
    void end_turn();

    // Accessors
    void on_game_over(std::function<void ()> callback) const;
    unsigned int turn() const;
    const Player* winner() const;
    const Player& current_player() const;
    const Player& next_player() const;

  private:
    mutable engine::event::CallbackList<void ()> _on_game_over;
    bool _is_game_over;

    const Player* _winner;
    std::array<std::unique_ptr<Player>, 2> _players;
    std::array<std::shared_ptr<Playmat>, 2> _playmats;
    int _current_player;
    int _turn;

    void start_turn();
  };

}
