#pragma once

#include "../../scene.hpp"
#include "../../card.hpp"
#include "../../texture.hpp"
#include "../../window.hpp"
#include "../../debug_camera.hpp"
#include "../../logger.hpp"
#include "../../pokemon_tcg_api.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

namespace open_pokemon_tcg::scenes {

  class DeckLoading : public IScene {
  public:
    DeckLoading(Window* window);
    ~DeckLoading();

    void update() override;
    void render() override;
    void gui() override;

  private:
    PokemonTcgApi* api;
    DebugCamera camera;
    std::vector<Card> cards;
    Shader *shader;

    void load_deck(std::string deck_id);
  };

  DeckLoading::DeckLoading(Window* window) :
    camera(DebugCamera(window,
                       Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))) {

    this->shader = new Shader("simple.vert", "simple.frag");

    this->api = new PokemonTcgApi();
    load_deck("Team Rocket");
  }
  DeckLoading::~DeckLoading() {}

  void DeckLoading::update() {}
  void DeckLoading::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    for (Card &c : cards)
      c.render(view_projection_matrix, this->shader);
  }

  void DeckLoading::gui() {
    // TODO: gui panel where you can choose which deck to load live.
  }

  void DeckLoading::load_deck(std::string deck_id) {
    nlohmann::json deck = this->api->load_deck(deck_id).data;

    this->cards.clear();
    int row = 0;
    for(auto &card : deck[0]["cards"]) {
      for(int i = 0; i < card["count"]; i++) {
        std::string id = card["id"];
        Texture tex = api->load_card(id).texture;

        this->cards.push_back(Card(Transform(glm::vec3(i, 0.0f, row)), tex.id()));
      }
      row++;
    }
  }
}
