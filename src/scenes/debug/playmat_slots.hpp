#pragma once

#include "../../scene.hpp"
#include "../../card.hpp"
#include "../../texture.hpp"
#include "../../window.hpp"
#include "../../debug_camera.hpp"
#include "../../playmats/black_playmat.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

namespace open_pokemon_tcg::scenes {

  class PlaymatSlots : public IScene {
  public:
    PlaymatSlots(Window* window);
    ~PlaymatSlots();

    void update() override;
    void render() override;
    void gui() override;

  private:
    DebugCamera camera;
    Shader *shader;
    IPlaymat *playmat;

    std::vector<Card> cards;
    Card debug_card;

    // GUI options
    bool show_cards = true;
    bool show_debug_card = false;
    glm::vec3 debug_card_pos = glm::vec3(0.0f);
    glm::vec3 debug_card_rot = glm::vec3(0.0f);
  };

  PlaymatSlots::PlaymatSlots(Window* window) :
    camera(DebugCamera(window,
                       Transform(glm::vec3(0.0f, 10.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))),
    debug_card(Card(Transform(glm::vec3(0.0f, 0.01f, 0.0f)), Texture("img/cardback.png").id())) {

    this->shader = new Shader("simple.vert", "simple.frag");
    this->playmat = new playmats::BlackPlaymat();

    this->cards.push_back(Card(playmat->active_slot(IPlaymat::PlayerSide::PLAYER1), Texture("cache/cards/img/base1-8.png").id()));
    this->cards.push_back(Card(playmat->supporter_slot(IPlaymat::PlayerSide::PLAYER1), Texture("cache/cards/img/base1-8.png").id()));
    this->cards.push_back(Card(playmat->stadium_slot(IPlaymat::PlayerSide::PLAYER1), Texture("cache/cards/img/base1-8.png").id()));
    for (int i = 0; i < 5; i++)
      this->cards.push_back(Card(playmat->bench_slots(IPlaymat::PlayerSide::PLAYER1)[i], Texture("cache/cards/img/base1-8.png").id()));
    for (int i = 0; i < 6; i++)
      this->cards.push_back(Card(playmat->prize_slots(IPlaymat::PlayerSide::PLAYER1)[i], Texture("cache/cards/img/base1-8.png").id()));
    this->cards.push_back(Card(playmat->deck_slot(IPlaymat::PlayerSide::PLAYER1), Texture("cache/cards/img/base1-8.png").id()));
    this->cards.push_back(Card(playmat->discard_slot(IPlaymat::PlayerSide::PLAYER1), Texture("cache/cards/img/base1-8.png").id()));

    this->cards.push_back(Card(playmat->active_slot(IPlaymat::PlayerSide::PLAYER2), Texture("cache/cards/img/base1-24.png").id()));
    this->cards.push_back(Card(playmat->supporter_slot(IPlaymat::PlayerSide::PLAYER2), Texture("cache/cards/img/base1-24.png").id()));
    this->cards.push_back(Card(playmat->stadium_slot(IPlaymat::PlayerSide::PLAYER2), Texture("cache/cards/img/base1-24.png").id()));
    for (int i = 0; i < 5; i++)
      this->cards.push_back(Card(playmat->bench_slots(IPlaymat::PlayerSide::PLAYER2)[i], Texture("cache/cards/img/base1-24.png").id()));
    for (int i = 0; i < 6; i++)
      this->cards.push_back(Card(playmat->prize_slots(IPlaymat::PlayerSide::PLAYER2)[i], Texture("cache/cards/img/base1-24.png").id()));
    this->cards.push_back(Card(playmat->deck_slot(IPlaymat::PlayerSide::PLAYER2), Texture("cache/cards/img/base1-24.png").id()));
    this->cards.push_back(Card(playmat->discard_slot(IPlaymat::PlayerSide::PLAYER2), Texture("cache/cards/img/base1-24.png").id()));
  }
  PlaymatSlots::~PlaymatSlots() {}

  void PlaymatSlots::update() {
    this->debug_card.transform.position = debug_card_pos;
    this->debug_card.transform.rotation = debug_card_rot;
  }

  void PlaymatSlots::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    if (show_debug_card)
      this->debug_card.render(view_projection_matrix, this->shader);

    if (show_cards)
      for (Card &c : cards)
        c.render(view_projection_matrix, this->shader);

    this->playmat->render(view_projection_matrix, this->shader);
  }

  void PlaymatSlots::gui() {
    ImGui::Begin("Playmat");
    ImGui::Checkbox("Show cards", &show_cards);
    ImGui::End();
    ImGui::Begin("Debug Card");
    ImGui::Checkbox("Show", &show_debug_card);
    ImGui::DragFloat3("position", (float*)&debug_card_pos, -10.0f      , 10.0f);
    ImGui::DragFloat3("rotation", (float*)&debug_card_rot, -2*glm::pi<float>(), 2*glm::pi<float>());
    ImGui::End();
  }
}
