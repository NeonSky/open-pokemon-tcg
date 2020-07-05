#include "player.hpp"

#include "../../engine/debug/logger.hpp"

#include <boost/lexical_cast.hpp>

using namespace open_pokemon_tcg::game::model;

Player::Player(std::unique_ptr<Deck>& deck, Playmat& playmat, std::string name)
  : _deck(std::move(deck)),
    _playmat(playmat),
    _name(name) {

  _playmat.deck_pile = new DeckPile(*_deck);
  _playmat.deck_pile->shuffle();

  _playmat.discard_pile = new DiscardPile();
  _playmat.bench = new Bench();

  std::array<std::reference_wrapper<ICard>, 6> prize_cards = {
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
    _playmat.deck_pile->pop(),
  };

  _playmat.prize_card_pool = new PrizeCardPool(prize_cards);
}

Player::~Player() {}

void Player::draw(unsigned int amount) {
  if (_playmat.deck_pile->size() < amount) {
    _on_lose();
    return;
  }

  for (unsigned int i = 0; i < amount; i++)
    _hand.add(_playmat.deck_pile->pop());
}

void Player::mill(unsigned int amount) {
  if (_playmat.deck_pile->size() < amount) {
    _on_lose();
    return;
  }

  for (unsigned int i = 0; i < amount; i++)
    _playmat.discard_pile->push(_playmat.deck_pile->pop());
}

void Player::discard(const ICard& card) {
  _playmat.discard_pile->push(_hand.take(_hand.find(card)));
}

void Player::place_active_pokemon(unsigned int hand_index) {
  if (hand_index >= _hand.size())
    LOG_ERROR("Hand size is " + std::to_string(_hand.size()) + ", so could not use card at index " + std::to_string(hand_index));

  if (_playmat.active_pokemon != nullptr)
    LOG_ERROR("Can not place active pokemon when there already is one.");

  _playmat.active_pokemon = (PokemonCard*) &_hand.cards()[hand_index];
  _hand.remove(hand_index);
}


void Player::place_on_active_slot_from_hand(const ICard& card) {
  if (_playmat.active_pokemon != nullptr)
    LOG_ERROR("Can not place an active pokemon when there already is one.");

  const PokemonCard* pokemon_card = dynamic_cast<const PokemonCard*>(&card);
  if (pokemon_card == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  _playmat.active_pokemon = dynamic_cast<PokemonCard*>(&_hand.take(_hand.find(card)));
  _on_update_active(_playmat.active_pokemon);
}

void Player::place_on_bench_from_hand(const ICard& card) {
  const PokemonCard* pokemon_card = dynamic_cast<const PokemonCard*>(&card);
  if (pokemon_card == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  _playmat.bench->place(*dynamic_cast<PokemonCard*>(&_hand.take(_hand.find(card))));
}

void Player::place_on_bench_from_hand(const ICard& card, unsigned int slot_index) {
  const PokemonCard* pokemon_card = dynamic_cast<const PokemonCard*>(&card);
  if (pokemon_card == nullptr)
    LOG_ERROR("Card must be a pokemon card.");

  _playmat.bench->place(*dynamic_cast<PokemonCard*>(&_hand.take(_hand.find(card))), slot_index);
}

void Player::activate_trainer_card(unsigned int hand_index) {
  LOG_DEBUG(hand_index);
  LOG_DEBUG("FIXME");
  /*try {
    TrainerCard *card = (TrainerCard*) &_hand.cards()[hand_index];
    _gm.activate(card->effect());
  } catch (const std::exception& e) {
    LOG_ERROR(e.what());
  }*/
}

void Player::take_prize_card() {
  _hand.add(_playmat.prize_card_pool->take_any());
  if (_playmat.prize_card_pool->empty())
    _on_win();
}


void Player::bench_pokemon_from_hand(const ICard& card) {
  ICard& c = _hand.take(_hand.find(card));
  _playmat.bench->place(c);
}

void Player::attach_to_active_from_hand(const BasicEnergy& energy_card) {
  if (_playmat.active_pokemon == nullptr)
    LOG_ERROR("There is no active pokemon to attach energy to.");

  _hand.remove(_hand.find(energy_card));
  _playmat.active_pokemon->attach_energy(energy_card);
}

void Player::remove_energy_from_active(const BasicEnergy& energy_card) {
  _playmat.active_pokemon->detach_energy(energy_card);
}

void Player::on_win(std::function<void ()> callback) const {
  _on_win.append(callback);
}

void Player::on_lose(std::function<void ()> callback) const {
  _on_lose.append(callback);
}

void Player::on_update_active(std::function<void (PokemonCard* card)> callback) const {
  _on_update_active.append(callback);
}

std::string Player::name() const {
  return _name;
}

const Deck& Player::deck() const {
  return *_deck;
}

const Playmat& Player::playmat() const {
  return _playmat;
}

const Hand& Player::hand() const {
  return _hand;
}
