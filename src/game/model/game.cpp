#include "game.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

Game::Game(std::array<Deck, 2> player_decks, std::array<std::string, 2> player_names) {
  _winner = nullptr;
  _current_player = 0;
  _turn = 0;

  for (unsigned int i = 0; i < _players.size(); i++) {
    _playmats[i] = std::make_shared<Playmat>();
    _players[i] = std::make_unique<Player>(Player(*this, player_decks[i], *_playmats[i], player_names[i]));
    _players[i]->draw(7);
  }

  start_turn();
}

Game::~Game() {}

const Player* Game::winner() const {
  return _winner;
}

const Player& Game::current_player() const {
  return *_players[_current_player];
}

const Player& Game::next_player() const {
  return *_players[(_current_player+1) % _players.size()];
}

unsigned int Game::turn() const {
  return _turn;
}

void Game::place_active_pokemon(unsigned int hand_index) {
  _players[_current_player]->place_active_pokemon(hand_index);
}

void Game::evolve_pokemon() {
  if (_turn == 0)
    LOG_ERROR("Not allowed to evolve pokemons on the first turn of the game.");
}

void Game::attack(unsigned int attack_index) {
  if (_turn == 0 && _current_player == 0)
    LOG_ERROR("The first player can not attack on the first turn.");

  effects::IHealthTarget *opponent = _players[(_current_player+1) % _players.size()]->playmat().active_pokemon;
  _players[_current_player]->playmat().active_pokemon->attack(attack_index, *opponent);
}

void Game::activate(effects::ICardEffect& effect) {
  // TODO: Need to get the player's models here instead.
  effect.activate(*_players[_current_player], *_players[(_current_player+1) % _players.size()]);
}

void Game::start_turn() {
  _players[_current_player]->draw();
  if (_players[_current_player]->lost())
    _winner = &next_player();
}

void Game::end_turn() {
  _current_player = (_current_player+1) % _players.size();

  start_turn();

  if (_current_player == 0)
    _turn++;
}
