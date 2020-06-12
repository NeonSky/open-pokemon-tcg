#include "duel_player.hpp"
#include "logger.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

using namespace open_pokemon_tcg;

DuelPlayer::DuelPlayer(Deck deck, const IPlaymat &playmat, IPlaymat::Side playmat_side)
  : deck(deck),
    playmat(&playmat),
    playmat_side(playmat_side) {

  std::vector<Card*> cards(this->deck.cards.size());
  for (unsigned int i = 0; i < cards.size(); i++)
    cards[i] = &this->deck.cards[i];

  this->deck_pile = new DeckPile(playmat.deck_slot(this->playmat_side), cards);
  this->deck_pile->shuffle();

  this->discard_pile = new DiscardPile(playmat.discard_slot(this->playmat_side));

  if (playmat_side == IPlaymat::Side::PLAYER1)
    this->hand = new Hand(Transform(glm::vec3(0.0f, 0.5f, -4.75f),
                                 glm::vec3(0.5f * glm::half_pi<float>(), 0.0f, 0.0f)));
  else
    this->hand = new Hand(Transform(glm::vec3(0.0f, 0.5f, 4.75f),
                                 glm::vec3(0.5f * glm::half_pi<float>(), glm::pi<float>(), 0.0f)));

  for (unsigned int i = 0; i < this->start_hand; i++)
    this->hand->cards.push_back(this->deck_pile->draw());

  std::array<Card*, 6> prize_cards;
  for (unsigned int i = 0; i < prize_cards.size(); i++)
    prize_cards[i] = this->deck_pile->draw();
  std::array<Transform, 6> prize_slots = this->playmat->prize_slots(this->playmat_side);

  this->prize_card_pool = new PrizeCardPool(prize_cards, this->playmat->prize_slots(this->playmat_side));
}

DuelPlayer::~DuelPlayer() {}

void DuelPlayer::update() {
  this->hand->update();
}

void DuelPlayer::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  this->deck_pile->render(view_projection_matrix, shader);
  this->discard_pile->render(view_projection_matrix, shader);
  this->hand->render(view_projection_matrix, shader);
  this->prize_card_pool->render(view_projection_matrix, shader);
}

void DuelPlayer::mill() {
  Card* card = this->deck_pile->draw();
  card->transform = this->playmat->discard_slot(this->playmat_side);
  this->discard_pile->cards.push_back(card);
}
