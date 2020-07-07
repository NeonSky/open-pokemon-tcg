#include "player.hpp"

#include "../../engine/debug/logger.hpp"
#include "playmat.hpp"

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <algorithm>
#include <iterator>

using namespace open_pokemon_tcg::game::view;

Player::Player(const model::Player &model, const IPlaymat &playmat, IPlaymat::Side playmat_side)
  : _model(model),
    playmat(&playmat),
    playmat_side(playmat_side) {

  _shader = new engine::graphics::Shader("simple.vert", "simple.frag");

  this->deck_pile = new DeckPile(*_model.playmat().deck_pile, playmat.deck_slot(this->playmat_side).transform());

  this->discard_pile = new DiscardPile(*_model.playmat().discard_pile, playmat.discard_slot(this->playmat_side).transform());

  if (playmat_side == open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1)
    this->hand = new Hand(_model.hand(), engine::geometry::Transform(glm::vec3(0.0f, 0.5f, -4.75f),
                                                 glm::vec3(0.5f * glm::half_pi<float>(), glm::pi<float>(), 0.0f)));
  else
    this->hand = new Hand(_model.hand(), engine::geometry::Transform(glm::vec3(0.0f, 0.5f, 4.75f),
                                                 glm::vec3(0.5f * glm::half_pi<float>(), 0.0f, 0.0f)));

  std::array<engine::geometry::Rectangle, 6> prize_slots = this->playmat->prize_slots(this->playmat_side);
  std::array<engine::geometry::Transform, 6> prize_slot_transforms;
  std::transform(prize_slots.begin(),
                prize_slots.end(),
                prize_slot_transforms.begin(),
                [](engine::geometry::Rectangle r) { return r.transform(); });

  this->prize_card_pool = new PrizeCardPool(*_model.playmat().prize_card_pool, prize_slot_transforms);

  this->bench = new Bench(*_model.playmat().bench, this->playmat->bench_slots(this->playmat_side));

  this->active_pokemon = nullptr;
  this->supporter_card = nullptr;
  this->stadium_card   = nullptr;

  _model.on_update_active(std::bind(&Player::on_update_active, this, std::placeholders::_1));
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

void Player::on_update_active(model::PokemonCard* card) {
  if (active_pokemon != nullptr) {
    delete active_pokemon;
    active_pokemon = nullptr;
  }

  if (card != nullptr)
    active_pokemon = new PokemonCard(*card, playmat->active_slot(this->playmat_side).transform());
}

/*
void Player::on_update_suppoter(model::TrainerCard* card) {
  if (supporter_card != nullptr)
    delete supporter_card;

  supporter_card = new Card(*card, playmat->supporter_slot(this->playmat_side).transform());
}

void Player::on_update_stadium(model::TrainerCard* card) {
  if (stadium_card != nullptr)
    delete stadium_card;

  stadium_card = new Card(*card, playmat->stadium_slot(this->playmat_side).transform());
}
 */
