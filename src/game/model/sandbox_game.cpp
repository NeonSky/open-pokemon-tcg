#include "sandbox_game.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

SandboxGame::SandboxGame(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names) {
  _winner = nullptr;
  _current_player = 0;
  _turn = 0;

  for (unsigned int i = 0; i < _players.size(); i++) {
    _playmats[i] = std::make_shared<Playmat>();
    _players[i] = std::make_unique<Player>(*this, player_decks[i], *_playmats[i], player_names[i]);
  }

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
}

SandboxGame::~SandboxGame() {}

// Mutators
void SandboxGame::perform(ICardEffect& effect) {
  effect.activate(*_players[_current_player], *_players[_next_player()]);
}

void SandboxGame::place_on_active_slot_from_hand(ICard& card) {
  _players[_current_player]->place_on_active_slot_from_hand(card);
}

void SandboxGame::place_on_bench_from_hand(ICard& card) {
  _players[_current_player]->place_on_bench_from_hand(card);
}

void SandboxGame::place_on_bench_from_hand(ICard& card, unsigned int slot_index) {
  _players[_current_player]->place_on_bench_from_hand(card, slot_index);
}

void SandboxGame::draw(unsigned int amount) {
  _players[_current_player]->draw(amount);
}

void SandboxGame::mill(unsigned int amount) {
  _players[_current_player]->mill(amount);
}

void SandboxGame::attack(unsigned int attack_index) {
  PokemonCard* my_active = _players[_current_player]->playmat().active_pokemon;
  if (my_active == nullptr)
    LOG_ERROR("There is no active pokemon to attack with.");

  IHealthTarget *opponent = _players[(_current_player+1) % _players.size()]->playmat().active_pokemon;
  if (opponent == nullptr)
    LOG_ERROR("The opponent has no active pokemon to attack.");

  my_active->attack(attack_index, *opponent);
}

// Accessors
void SandboxGame::on_game_over(std::function<void ()> callback) const {
  _on_game_over.append(callback);
}

void SandboxGame::end_turn() {
  if (_winner != nullptr)
    LOG_ERROR("The game is already over.")

  _current_player = _next_player();

  if (_current_player == 0)
    _turn++;
}

unsigned int SandboxGame::turn() const {
  return _turn;
}

const Player* SandboxGame::winner() const {
  return _winner;
}

const Player& SandboxGame::current_player() const {
  return *_players[_current_player];
}

const Player& SandboxGame::next_player() const {
  return *_players[_next_player()];
}

// Private
unsigned int SandboxGame::_next_player() const {
  return (_current_player+1) % _players.size();
}
