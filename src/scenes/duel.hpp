#pragma once

#include "../scene.hpp"
#include "../card.hpp"
#include "../texture.hpp"
#include "../window.hpp"
#include "../debug_camera.hpp"
#include "../deck.hpp"
#include "../discard_pile.hpp"
#include "../hand.hpp"
#include "../pokemon_tcg_api.hpp"
#include "../playmats/black_playmat.hpp"
#include "../logger.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

namespace open_pokemon_tcg::scenes {

  class Duel : public IScene {
  public:
    Duel(Window* window);
    ~Duel();

    // Mutators
    void update() override;
    void render() override;
    void gui() override;

  private:
    DebugCamera camera;

    // TODO: Move these to player class
    Deck *deck;
    DiscardPile *discard_pile;
    Hand *hand;

    Shader *shader;
    IPlaymat *playmat;

    // Mutators
    void on_key(GLFWwindow* window);
  };

  Duel::Duel(Window* window) :
    camera(DebugCamera(window,
                       Transform(glm::vec3(0.0f, 7.0f, -8.0f),
                                 glm::vec3(-0.5f*glm::half_pi<float>(), 0.0f, 0.0f)))) {

    window->add_on_key_callback(std::bind(&Duel::on_key, this, std::placeholders::_1));

    this->shader = new Shader("simple.vert", "simple.frag");

    // Player player1;
    // Player player2;
    this->playmat = new playmats::BlackPlaymat();

    PokemonTcgApi *api = new PokemonTcgApi();
    nlohmann::json deck_data = api->load_deck("Base").data;

    std::vector<Card> cards;
    for (auto &card : deck_data[0]["cards"])
      for (int i = 0; i < card["count"]; i++)
        cards.push_back(Card(Transform(), api->load_card((std::string)card["id"]).texture.id()));

    this->deck = new Deck(this->playmat->deck_slot(IPlaymat::PlayerSide::PLAYER1), cards);
    this->deck->shuffle();
    this->discard_pile = new DiscardPile(this->playmat->discard_slot(IPlaymat::PlayerSide::PLAYER1));

    // TODO: maybe place relative to camera instead.
    this->hand = new Hand(Transform(glm::vec3(0.0f, 2.0f, -5.5f), glm::vec3(0.5f * glm::half_pi<float>(), 0.0f, 0.0f)));
  }
  Duel::~Duel() {}

  void Duel::update() {
    this->hand->update();
  }

  void Duel::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    this->playmat->render(view_projection_matrix, this->shader);
    this->deck->render(view_projection_matrix, this->shader);
    this->discard_pile->render(view_projection_matrix, this->shader);
    this->hand->render(view_projection_matrix, this->shader);
  }

  void Duel::gui() {}

  void Duel::on_key(GLFWwindow* window) {
    // Mill
    if (glfwGetKey(window, GLFW_KEY_M)) {
      Card card = this->deck->draw();
      card.transform = this->playmat->discard_slot(IPlaymat::PlayerSide::PLAYER1);
      this->discard_pile->add_on_top(card);
    }

    // Hand
    if (glfwGetKey(window, GLFW_KEY_H)) {
      this->hand->cards.push_back(this->deck->draw());
    }
  }
}
