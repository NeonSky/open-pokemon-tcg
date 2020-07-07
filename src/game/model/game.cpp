#include "game.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

Game::Game(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names) {
  _sandbox_game = std::make_unique<SandboxGame>(player_decks, player_names);
  _current_player().draw(7);
  _next_player().draw(7);
  start_turn();
}

Game::~Game() {}

// Mutators
void Game::place_on_active_slot_from_hand(const ICard& card) {
  // TODO: Check if pokemon card.
  // TODO: Get index of card.
  _sandbox_game->place_on_active_slot_from_hand(card);
}

void Game::place_on_bench_from_hand(const ICard& card) {
  // TODO: Check if pokemon card.
  _sandbox_game->place_on_bench_from_hand(card);
}

void Game::place_on_bench_from_hand(const ICard& card, unsigned int slot_index) {
  // TODO: Check if pokemon card.
  _sandbox_game->place_on_bench_from_hand(card, slot_index);
}

void Game::activate_trainer_card(const ICard& card, std::vector<std::reference_wrapper<const ICard>> targets) {
  _sandbox_game->activate_trainer_card(card, targets);
}

void Game::attach_to_active_pokemon(const ICard& card) {
  _sandbox_game->attach_to_active_pokemon(card);
}

void Game::attach_to_bench_pokemon(const ICard& card, unsigned int slot_index) {
  _sandbox_game->attach_to_bench_pokemon(card, slot_index);
}

void Game::evolve_pokemon() {
  if (_sandbox_game->_turn == 0)
    LOG_ERROR("Not allowed to evolve pokemons on the first turn of the game.");
}

void Game::attack(unsigned int attack_index) {
  if (_sandbox_game->_turn == 0 && _sandbox_game->_current_player == 0)
    LOG_ERROR("The first player can not attack on the first turn.");

  _sandbox_game->attack(attack_index);
  start_turn();
}

void Game::end_turn() {
  if (_sandbox_game->_winner != nullptr)
    return;

  _sandbox_game->end_turn();
  start_turn();
}

// Accessors
void Game::on_game_over(std::function<void ()> callback) const {
  _sandbox_game->_on_game_over.append(callback);
}

const Player* Game::winner() const {
  return _sandbox_game->winner();
}

const Player& Game::current_player() const {
  return _sandbox_game->current_player();
}

const Player& Game::next_player() const {
  return _sandbox_game->next_player();
}

unsigned int Game::turn() const {
  return _sandbox_game->turn();
}

// Private
void Game::start_turn() {
  _sandbox_game->draw();
}

Player& Game::_current_player() {
  return *_sandbox_game->_players[_sandbox_game->_current_player];
}

Player& Game::_next_player() {
  return *_sandbox_game->_players[_sandbox_game->_next_player()];
}
