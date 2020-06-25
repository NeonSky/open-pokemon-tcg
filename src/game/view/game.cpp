#include "game.hpp"
#include "playmat.hpp"

using namespace open_pokemon_tcg::game::view;

Game::Game(std::shared_ptr<model::Game> model, open_pokemon_tcg::game::view::IPlaymat &playmat) : _model(model), _playmat(playmat) {
  _players[0] = std::make_unique<Player>(_model->current_player(), playmat, open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER1);
  _players[1] = std::make_unique<Player>(_model->next_player(), playmat, open_pokemon_tcg::game::view::IPlaymat::Side::PLAYER2);
}

Game::~Game() {}

void Game::render(const glm::mat4 &view_projection) {
  _playmat.render(view_projection);
  for (auto &player : _players)
    player->render(view_projection);
}

open_pokemon_tcg::game::model::Game& Game::model() { return *_model; }

std::array<Player*, 2> Game::players() {
  return { _players[0].get(), _players[1].get() };
}
