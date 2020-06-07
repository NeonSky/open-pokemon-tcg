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

namespace open_pokemon_tcg {

  class CardRotation : public IScene {
  public:
    CardRotation(Window* window);
    ~CardRotation();

    void update() override;
    void render() override;
    void gui() override;

  private:
    DebugCamera camera;
    std::vector<Card> cards;
    Card *debug_card;
    Shader *shader;

    glm::vec3 pos = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3 rot;
    glm::vec3 scale = glm::vec3(1.0f);
  };



  CardRotation::CardRotation(Window* window) : camera(
                                                     DebugCamera(window,
                                                                 Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                                                           glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))) {

    this->shader = new Shader("simple.vert", "simple.frag");

    PokemonTcgApi* api = new PokemonTcgApi();

    // TODO: migrate to new scene where we test loading decks.
    nlohmann::json deck = api->load_deck("Team Rocket").data;
    int row = 0;
    for(auto &card : deck[0]["cards"]) {
      for(int i = 0; i < card["count"]; i++) {
        std::string id = card["id"];
        Texture tex = api->load_card(id).texture;

        this->cards.push_back(Card(Transform(glm::vec3(i, 0.0f, row)), tex.id()));
      }
      row++;
    }

    debug_card = new Card(Transform(), Texture("base1-26.png").id());

  }
  CardRotation::~CardRotation() {}

  void CardRotation::update() {
    this->debug_card->transform.position = pos;
    this->debug_card->transform.rotation = rot;
    this->debug_card->transform.scale = scale;
  }

  void CardRotation::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    for (Card &c : cards) {
      c.render(view_projection_matrix, this->shader);
    }

    this->debug_card->render(view_projection_matrix, shader);
  }

  void CardRotation::gui() {
    ImGui::Begin("Transform Debugging");
    ImGui::SliderFloat3("position", (float*)&pos, 0.0f, 10.0f);
    ImGui::SliderFloat3("rotation", (float*)&rot, -2*glm::pi<float>(), 2*glm::pi<float>());
    ImGui::SliderFloat3("scale", (float*)&scale, 0.1f, 10.0f);
    ImGui::End();
  }
}
