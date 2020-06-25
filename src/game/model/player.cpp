#include "player.hpp"

#include "cards/trainer_card.hpp"
#include "../../engine/debug/logger.hpp"

#include <boost/lexical_cast.hpp>

using namespace open_pokemon_tcg::game::model;

Player::Player(IGameMaster &gm, Deck deck, Playmat &playmat, std::string name) : _gm(gm), _deck(deck), _playmat(playmat), _name(name) {
  _lost = false;

  _playmat.deck_pile.resize(_deck.cards.size());
  for (unsigned int i = 0; i < _deck.cards.size(); i++)
    _playmat.deck_pile[i] = _deck.cards[i];
}

Player::~Player() {}

void Player::draw(unsigned int amount) {
  if (_playmat.deck_pile.size() < amount) {
    _lost = true;
    return;
  }

  for (unsigned int i = 0; i < amount; i++) {
    _hand.push_back(_playmat.deck_pile.back());
    _playmat.deck_pile.pop_back();
  }
}

void Player::place_active_pokemon(unsigned int hand_index) {
  if (hand_index >= _hand.size())
    LOG_ERROR("Hand size is " + std::to_string(_hand.size()) + ", so could not use card at index " + std::to_string(hand_index));

  if (_playmat.active_pokemon != nullptr)
    LOG_ERROR("Can not place active pokemon when there already is one.");
  _playmat.active_pokemon = (cards::PokemonCard*) _hand[hand_index];

  _hand.erase(_hand.begin() + hand_index);
}

void Player::activate_trainer_card(unsigned int hand_index) {
  try {
    cards::TrainerCard *card = (cards::TrainerCard*) _hand[hand_index];
    _gm.activate(card->effect());
  } catch (const std::exception& e) {
    LOG_ERROR(e.what());
  }
}

bool Player::lost() const {
  return _lost;
}

std::string Player::name() const {
  return _name;
}

const Playmat& Player::playmat() const {
  return _playmat;
}

const Deck& Player::deck() const {return _deck;}

const std::vector<ICard*>& Player::hand() const {return _hand;}
