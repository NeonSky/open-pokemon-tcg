#pragma once

#include "../../scene.hpp"
#include "../../card.hpp"
#include "../../texture.hpp"
#include "../../window.hpp"
#include "../../debug_camera.hpp"

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
    Card debug_card;
    Shader *shader;

    glm::vec3 pos = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3 rot;
    glm::vec3 scale = glm::vec3(1.0f);
  };



  CardRotation::CardRotation(Window* window) : camera(
                                                     DebugCamera(window,
                                                                 Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                                                           glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))),
                                              debug_card(Card(Transform(), Texture("test3.png").id())) {

    srand(time(NULL));
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 2; j++) {
        int ind = (rand() % 2) + 1;
        float x = (float) i;
        float z = (float) j;
        this->cards.push_back(Card(Transform(glm::vec3(x, 0.0f, z)), Texture("test" + std::to_string(ind) + ".png").id()));
      }
    }

    this->shader = new Shader("simple.vert", "simple.frag");
  }
  CardRotation::~CardRotation() {}

  void CardRotation::update() {
    this->debug_card.transform.position = pos;
    this->debug_card.transform.rotation = rot;
    this->debug_card.transform.scale = scale;
  }

  void CardRotation::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    for (Card &c : cards) {
      c.render(view_projection_matrix, this->shader);
    }

    this->debug_card.render(view_projection_matrix, shader);
  }

  void CardRotation::gui() {
    ImGui::Begin("Transform Debugging");
    ImGui::SliderFloat3("position", (float*)&pos, 0.0f, 10.0f);
    ImGui::SliderFloat3("rotation", (float*)&rot, -2*glm::pi<float>(), 2*glm::pi<float>());
    ImGui::SliderFloat3("scale", (float*)&scale, 0.1f, 10.0f);
    ImGui::End();
  }
}
