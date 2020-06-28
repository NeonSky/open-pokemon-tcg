#include "player.hpp"

#include "../../engine/debug/logger.hpp"
#include "playmat.hpp"

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <algorithm>
#include <iterator>

using namespace open_pokemon_tcg::game::view;

Player::Player(const model::Player &model, const open_pokemon_tcg::game::view::IPlaymat &playmat, open_pokemon_tcg::game::view::IPlaymat::Side playmat_side)
  : _model(model),
    deck(Deck(model.deck())),
    playmat(&playmat),
    playmat_side(playmat_side) {

  _shader = new engine::graphics::Shader("simple.vert", "simple.frag");

  std::vector<Card*> cards(this->deck.cards.size());
  for (unsigned int i = 0; i < cards.size(); i++)
    cards[i] = &this->deck.cards[i];

  this->deck_pile = new DeckPile(playmat.deck_slot(this->playmat_side).transform(), cards);
  this->deck_pile->shuffle();

  this->discard_pile = new DiscardPile(playmat.discard_slot(this->playmat_side).transform());

  if (playmat_side == open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1)
    this->hand = new Hand(engine::geometry::Transform(glm::vec3(0.0f, 0.5f, -4.75f),
                                                 glm::vec3(0.5f * glm::half_pi<float>(), glm::pi<float>(), 0.0f)));
  else
    this->hand = new Hand(engine::geometry::Transform(glm::vec3(0.0f, 0.5f, 4.75f),
                                                 glm::vec3(0.5f * glm::half_pi<float>(), 0.0f, 0.0f)));

  for (unsigned int i = 0; i < this->start_hand; i++)
    this->hand->cards.push_back(this->deck_pile->draw());

  std::array<Card*, 6> prize_cards;
  for (unsigned int i = 0; i < prize_cards.size(); i++)
    prize_cards[i] = this->deck_pile->draw();

  std::array<engine::geometry::Rectangle, 6> prize_slots = this->playmat->prize_slots(this->playmat_side);
  std::array<engine::geometry::Transform, 6> prize_slot_transforms;
  std::transform(prize_slots.begin(),
                prize_slots.end(),
                prize_slot_transforms.begin(),
                [](engine::geometry::Rectangle r) { return r.transform(); });

  this->prize_card_pool = new PrizeCardPool(prize_cards, prize_slot_transforms);

  this->bench = new Bench(this->playmat->bench_slots(this->playmat_side));

  this->active_pokemon = nullptr;
  this->supporter_card = nullptr;
  this->stadium_card   = nullptr;
}

Player::~Player() {}

void Player::update() {
  this->hand->update();
}

void Player::render(const glm::mat4 &view_projection) {
  _shader->use();

  this->deck_pile->render(view_projection, _shader);
  this->discard_pile->render(view_projection, _shader);
  this->hand->render(view_projection, _shader);
  this->prize_card_pool->render(view_projection, _shader);
  this->bench->render(view_projection, _shader);

  if (this->active_pokemon != nullptr)
    this->active_pokemon->render(view_projection, _shader);
  if (this->supporter_card != nullptr)
    this->supporter_card->render(view_projection, _shader);
  if (this->stadium_card != nullptr)
    this->stadium_card->render(view_projection, _shader);
}

void Player::mill() {
  Card* card = this->deck_pile->draw();
  card->transform = this->playmat->discard_slot(this->playmat_side).transform();
  this->discard_pile->cards.push_back(card);
}

void Player::place_active_pokemon(Card *card) {
  card->transform = this->playmat->active_slot(this->playmat_side).transform();
  this->active_pokemon = card;
  this->hand->remove(card);
}

void Player::place_bench_pokemon(Card *card) {
  this->bench->place(card);
  this->hand->remove(card);
}