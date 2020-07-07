#pragma once

#include "sandbox_game.hpp"

namespace open_pokemon_tcg::game::model {

  class Game {
  public:
    Game(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names);
    ~Game();

    // Mutators
    void place_on_active_slot_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card, unsigned int slot_index);
    void activate_trainer_card(const ICard& card, std::vector<std::reference_wrapper<const ICard>> targets);

    void attach_to_active_pokemon(const ICard& card);
    void attach_to_bench_pokemon(const ICard& card, unsigned int slot_index);
    void evolve_pokemon();

    bool attack(unsigned int attack_index);
    void end_turn();

    // Accessors
    void on_game_over(std::function<void ()> callback) const;
    unsigned int turn() const;
    const Player* winner() const;
    const Player& current_player() const;
    const Player& next_player() const;

  private:
    std::unique_ptr<SandboxGame> _sandbox_game;

    void start_turn();
    Player& _current_player();
    Player& _next_player();
  };

}
