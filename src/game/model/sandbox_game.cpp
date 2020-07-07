#include "sandbox_game.hpp"
#include "card/item.hpp"
#include "card/basic_energy.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::model;

SandboxGame::SandboxGame(std::array<std::unique_ptr<Deck>, 2>& player_decks, std::array<std::string, 2> player_names) {
  _winner = nullptr;
  _current_player = 0;
  _turn = 0;

  for (unsigned int i = 0; i < _players.size(); i++) {
    _playmats[i] = std::make_shared<Playmat>();
    _players[i] = std::make_unique<Player>(player_decks[i], *_playmats[i], player_names[i]);
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
void SandboxGame::place_on_active_slot_from_hand(const ICard& card) {
  _players[_current_player]->place_on_active_slot_from_hand(card);
}

void SandboxGame::place_on_bench_from_hand(const ICard& card) {
  _players[_current_player]->place_on_bench_from_hand(card);
}

void SandboxGame::place_on_bench_from_hand(const ICard& card, unsigned int slot_index) {
  _players[_current_player]->place_on_bench_from_hand(card, slot_index);
}

void SandboxGame::attack(unsigned int attack_index) {
  const PokemonCard& active = *_players[_current_player]->playmat().active_pokemon;
  const Attack* attack = active.data().attacks[attack_index];
  IAttackEffect* attack_effect = attack->effect();

  unsigned int instance_damage = attack->damage();
  if (attack_effect != nullptr) {
    if (attack_effect->required_targets().size() > 0)
      LOG_ERROR("Required targets for attack effects is not supported yet.");

    attack_effect->activate(*_players[_current_player], *_players[_next_player()], {}, instance_damage);
  }

  _players[_next_player()]->apply_attack_damage(instance_damage, _players[_current_player]->playmat().active_pokemon->data().energy_type);
  end_turn();
}

void SandboxGame::activate_trainer_card(const ICard& card, std::vector<std::reference_wrapper<const ICard>> targets) {
  const ItemCard* item_card = dynamic_cast<const ItemCard*>(&card);
  if (item_card == nullptr)
    LOG_ERROR("Card must be an item card.");

  ICardEffect& effect = item_card->effect();

  if (!effect.can_activate(*_players[_current_player], *_players[_next_player()]))
    LOG_ERROR("Can't activate this card's effect.");

  _players[_current_player]->discard(card);
  effect.activate(*_players[_current_player], *_players[_next_player()], targets);
}

void SandboxGame::attach_to_active_pokemon(const ICard& card) {
  const BasicEnergy* energy_card = dynamic_cast<const BasicEnergy*>(&card);
  if (energy_card == nullptr)
    LOG_ERROR("Card must be an energy card.");

  _players[_current_player]->attach_to_active_from_hand(*energy_card);
}

void SandboxGame::attach_to_bench_pokemon(const ICard& card, unsigned int slot_index) {
  const BasicEnergy* energy_card = dynamic_cast<const BasicEnergy*>(&card);
  if (energy_card == nullptr)
    LOG_ERROR("Card must be an energy card.");

  _players[_current_player]->attach_to_bench_slot_from_hand(*energy_card, slot_index);
}

void SandboxGame::draw(unsigned int amount) {
  _players[_current_player]->draw(amount);
}

void SandboxGame::mill(unsigned int amount) {
  _players[_current_player]->mill(amount);
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
