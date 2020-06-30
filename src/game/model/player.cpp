#include "player.hpp"

#include "card/trainer_card.hpp"
#include "../../engine/debug/logger.hpp"

#include <boost/lexical_cast.hpp>

using namespace open_pokemon_tcg::game::model;

Player::Player(IGameMaster &gm, Deck deck, Playmat &playmat, std::string name) : _gm(gm), _deck(deck), _playmat(playmat), _name(name) {
  _lost = false;
  _won = false;

  _playmat.deck_pile = new DeckPile(_deck.cards);
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
    _lost = true;
    return;
  }

  for (unsigned int i = 0; i < amount; i++)
    _hand.add(_playmat.deck_pile->pop());
}

void Player::mill(unsigned int amount) {
  if (_playmat.deck_pile->size() < amount) {
    _lost = true;
    return;
  }

  for (unsigned int i = 0; i < amount; i++)
    _playmat.discard_pile->push(_playmat.deck_pile->pop());
}

void Player::place_active_pokemon(unsigned int hand_index) {
  if (hand_index >= _hand.size())
    LOG_ERROR("Hand size is " + std::to_string(_hand.size()) + ", so could not use card at index " + std::to_string(hand_index));

  if (_playmat.active_pokemon != nullptr)
    LOG_ERROR("Can not place active pokemon when there already is one.");

  _playmat.active_pokemon = (PokemonCard*) &_hand.cards()[hand_index];
  _hand.remove(hand_index);
}


void Player::active_pokemon_from_hand(ICard& card) {
  if (_playmat.active_pokemon != nullptr)
    LOG_ERROR("Can not place an active pokemon when there already is one.");

  PokemonCard* pokemon_card = dynamic_cast<PokemonCard*>(&card);
  if (pokemon_card == nullptr)
    LOG_ERROR("Card must be a pokemon card.")

  _hand.remove(_hand.find(card));
  _playmat.active_pokemon = pokemon_card;
  _on_update_active(_playmat.active_pokemon);
}

void Player::activate_trainer_card(unsigned int hand_index) {
  try {
    TrainerCard *card = (TrainerCard*) &_hand.cards()[hand_index];
    _gm.activate(card->effect());
  } catch (const std::exception& e) {
    LOG_ERROR(e.what());
  }
}

void Player::take_prize_card() {
  _hand.add(_playmat.prize_card_pool->take_any());
  if (_playmat.prize_card_pool->empty())
    _won = true;
}


void Player::bench_pokemon_from_hand(ICard& card) {
  _hand.remove(_hand.find(card));
  _playmat.bench->place(card);
}

void Player::on_update_active(std::function<void (PokemonCard* card)> callback) {
  _on_update_active.append(callback);
}

void Player::on_update_supporter(std::function<void (TrainerCard* card)> callback) {
  _on_update_supporter.append(callback);
}

void Player::on_update_stadium(std::function<void (TrainerCard* card)> callback) {
  _on_update_stadium.append(callback);
}

bool Player::lost() const {
  return _lost;
}

bool Player::won() const {
  return _won;
}

std::string Player::name() const {
  return _name;
}

Playmat& Player::playmat() {
  return _playmat;
}

const Deck& Player::deck() const {return _deck;}

Hand& Player::hand() {return _hand;}
