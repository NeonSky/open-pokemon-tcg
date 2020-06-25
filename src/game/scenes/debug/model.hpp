#pragma once

#include "../../model/game.hpp"
#include "../../card_loader.hpp"

#include "../../../engine/debug/logger.hpp"
#include "../../../engine/gui/window.hpp"
#include "../../../engine/scene/scene.hpp"

namespace open_pokemon_tcg::game::scenes {

  class Model : public engine::scene::IScene {
  public:
    Model(engine::gui::Window* window);
    ~Model();

    void update() override;
    void render() override;
    void gui() override;

  private:
    bool _end_game = false;
    std::string PLAYER1 = "Alice";
    std::string PLAYER2 = "Bob";

    game::model::Game *_game;
    std::vector<game::model::Deck> _decks;

    void start_turn();
    void display_options();
    void on_key(GLFWwindow* window, int);
  };

  Model::Model(engine::gui::Window* window) {
    window->add_on_key_callback(std::bind(&Model::on_key, this, std::placeholders::_1, std::placeholders::_2));

    _decks = std::vector<game::model::Deck>(2);
    game::CardLoader loader;

    _decks[0].name = PLAYER1 + "'s deck";
    for (int i = 0; i < 10; i++)
      _decks[0].cards.push_back(new game::model::PokemonCard(loader.load_pokemon_card("base1-26")));

    _decks[1].name = PLAYER2 + "'s deck";
    for (int i = 0; i < 10; i++)
      _decks[1].cards.push_back(new game::model::PokemonCard(loader.load_pokemon_card("base1-26")));

    _game = new game::model::Game(_decks, {PLAYER1, PLAYER2});
    start_turn();
  }

  Model::~Model() {}

  void Model::update() {}
  void Model::render() {}
  void Model::gui() {}

  void Model::display_options() {
    LOG_DEBUG("### Options ###");
    LOG_DEBUG("[A]ctive");
    LOG_DEBUG("[D]deck");
    LOG_DEBUG("[E]nd turn");
    LOG_DEBUG("[H]and");
    LOG_DEBUG("[K]ill game");
    LOG_DEBUG("[M]y playmat");
    LOG_DEBUG("[O]pponent's playmat");
    LOG_DEBUG("");
  }

  void Model::start_turn() {
    if (_game->current_player().name() == PLAYER1)
      LOG_DEBUG("\n\n### Turn: " + std::to_string(_game->turn()) + " ###");
    LOG_DEBUG("Player: " + _game->current_player().name() + "\n");

    display_options();
  }

  void Model::on_key(GLFWwindow* window, int action) {
    if (action != GLFW_PRESS || _end_game)
      return;

    const game::model::Player& player = _game->current_player();

    if (glfwGetKey(window, GLFW_KEY_D)) {
      LOG_DEBUG("Cards in deck: " + std::to_string(player.playmat().deck_pile.size()));
      for (unsigned int i = 0; i < player.playmat().deck_pile.size(); i++)
        LOG_DEBUG(std::to_string(i+1) + ". " + player.playmat().deck_pile[i]->debug());
    }

    if (glfwGetKey(window, GLFW_KEY_E)) {
      LOG_DEBUG("Ended turn");
      _game->end_turn();
      start_turn();
    }

    if (glfwGetKey(window, GLFW_KEY_H)) {
      LOG_DEBUG("Cards in hand: " + std::to_string(player.hand().size()));
      for (unsigned int i = 0; i < player.hand().size(); i++)
        LOG_DEBUG(std::to_string(i+1) + ". " + player.hand()[i]->debug());
    }

    if (glfwGetKey(window, GLFW_KEY_K)) {
      LOG_DEBUG("Forced game to end!");
      _end_game = true;
    }
  }
}
