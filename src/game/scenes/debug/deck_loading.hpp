#pragma once

#include "../../view/card.hpp"
#include "../debug_camera.hpp"
#include "../../data/pokemon_tcg_api.hpp"

#include "../../../engine/debug/logger.hpp"
#include "../../../engine/graphics/texture.hpp"
#include "../../../engine/gui/window.hpp"
#include "../../../engine/scene/scene.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

namespace open_pokemon_tcg::game::scenes {

class DeckLoading : public engine::scene::IScene {
  public:
    DeckLoading(engine::gui::Window* window);
    ~DeckLoading();

    void update() override;
    void render() override;
    void gui() override;

  private:
    PokemonTcgApi* api;
    DebugCamera camera;
    std::vector<Card> cards;
    engine::graphics::Shader *shader;

    // GUI options
    char deck_id[100] = { 0 };
    bool one_card_type_per_row = false;

    void load_deck(std::string deck_id);
  };

  DeckLoading::DeckLoading(engine::gui::Window* window) :
    camera(DebugCamera(window,
                       engine::geometry::Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))) {

    this->shader = new engine::graphics::Shader("simple.vert", "simple.frag");

    this->api = new PokemonTcgApi();
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
    ImGui::Begin("Deck Load Debugging");
    ImGui::InputText("Deck ID", deck_id, IM_ARRAYSIZE(deck_id));
    if (ImGui::Button("Load Deck"))
      load_deck(deck_id);
    ImGui::Checkbox("One card type per row", &one_card_type_per_row);
    ImGui::End();
  }

  void DeckLoading::load_deck(std::string deck_id) {
    nlohmann::json deck = this->api->load_deck(deck_id).data;
    this->cards.clear();

    int card_counter = 0;
    int row = 0;
    for(auto &card : deck[0]["cards"]) {
      for(int i = 0; i < card["count"]; i++) {
        std::string id = card["id"];
        engine::graphics::Texture tex = api->load_card(id).texture;

        if (one_card_type_per_row)
          this->cards.push_back(Card(engine::geometry::Transform(glm::vec3(i, 0.0f, row)), tex.id()));
        else
          this->cards.push_back(Card(engine::geometry::Transform(glm::vec3(card_counter % 10, 0.0f, card_counter / 10)), tex.id()));

        card_counter++;
      }
      row++;
    }

    LOG_INFO("This deck contains " + std::to_string(card_counter) + "cards.");
  }
}
