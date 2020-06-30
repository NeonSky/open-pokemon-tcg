#pragma once

#include "player.hpp"
#include "playmat.hpp"

#include <memory>
#include <string>
#include <array>

namespace open_pokemon_tcg::game::model {

  class Game : IGameMaster {
  public:
    Game(std::array<Deck, 2> player_decks, std::array<std::string, 2> player_names);
    ~Game();

    // Mutators
    void place_active_pokemon(unsigned int hand_index);
    void evolve_pokemon();
    void attack(unsigned int attack_index);
    void activate(ICardEffect& effect) override;
    void end_turn();

    // Accessors
    const Player* winner() const;
    Player& current_player();
    Player& next_player();
    unsigned int turn() const;

  private:
    const Player* _winner;
    std::array<std::unique_ptr<Player>, 2> _players;
    std::array<std::shared_ptr<Playmat>, 2> _playmats;
    int _current_player;
    int _turn;

    void start_turn();
  };

}
