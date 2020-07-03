#include "game.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

Game::Game(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names) {
  _is_game_over = false;
  _winner = nullptr;
  _current_player = 0;
  _turn = 0;

  for (unsigned int i = 0; i < _players.size(); i++) {
    _playmats[i] = std::make_shared<Playmat>();
    _players[i] = std::make_unique<Player>(*this, player_decks[i], *_playmats[i], player_names[i]);
    _players[i]->draw(7);
  }

  on_game_over([this]() { _is_game_over = true; });

  _players[0]->on_win([this]() {
    _winner = _players[0].get();
    _on_game_over();
  });

  _players[1]->on_win([this]() {
    _winner = _players[1].get();
    _on_game_over();
  });

  _players[0]->on_lose([this]() {
    _winner = _players[1].get();
    _on_game_over();
  });

  _players[1]->on_lose([this]() {
    _winner = _players[0].get();
    _on_game_over();
  });

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

void Game::place_on_active_slot_from_hand(ICard& card) {
  // TODO: Check if pokemon card.
  // TODO: Get index of card.
  _players[_current_player]->place_on_active_slot_from_hand(card);
}

void Game::place_on_bench_from_hand(ICard& card) {
  _players[_current_player]->place_on_bench_from_hand(card);
}

void Game::place_on_bench_slot_from_hand(ICard& card, unsigned int slot_index) {
  _players[_current_player]->place_on_bench_from_hand(card, slot_index);
}

void Game::evolve_pokemon() {
  if (_turn == 0)
    LOG_ERROR("Not allowed to evolve pokemons on the first turn of the game.");
}

void Game::attack(unsigned int attack_index) {
  if (_turn == 0 && _current_player == 0)
    LOG_ERROR("The first player can not attack on the first turn.");

  IHealthTarget *opponent = _players[(_current_player+1) % _players.size()]->playmat().active_pokemon;
  _players[_current_player]->playmat().active_pokemon->attack(attack_index, *opponent);
}

void Game::activate(ICardEffect& effect) {
  // TODO: Need to get the player's models here instead.
  effect.activate(*_players[_current_player], *_players[(_current_player+1) % _players.size()]);
}

void Game::start_turn() {
  _players[_current_player]->draw();
}

void Game::on_game_over(std::function<void ()> callback) const {
  _on_game_over.append(callback);
}

void Game::end_turn() {
  if (_is_game_over) return;

  _current_player = (_current_player+1) % _players.size();
  start_turn();

  if (_current_player == 0)
    _turn++;
}
