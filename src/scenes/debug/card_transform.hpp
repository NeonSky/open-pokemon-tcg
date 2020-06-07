#pragma once

#include "../../scene.hpp"
#include "../../card.hpp"
#include "../../texture.hpp"
#include "../../window.hpp"
#include "../../debug_camera.hpp"
#include "../../logger.hpp"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

namespace open_pokemon_tcg::scenes {

  class CardTransform : public IScene {
  public:
    CardTransform(Window* window);
    ~CardTransform();

    void update() override;
    void render() override;
    void gui() override;

  private:
    DebugCamera camera;
    Card *debug_card;
    Shader *shader;

    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 rot = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
  };

  CardTransform::CardTransform(Window* window) :
    camera(DebugCamera(window,
                       Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))) {

    this->shader = new Shader("simple.vert", "simple.frag");
    debug_card = new Card(Transform(), Texture("base1-26.png").id());

  }
  CardTransform::~CardTransform() {}

  void CardTransform::update() {
    this->debug_card->transform.position = pos;
    this->debug_card->transform.rotation = rot;
    this->debug_card->transform.scale = scale;
  }

  void CardTransform::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    this->debug_card->render(view_projection_matrix, shader);
  }

  void CardTransform::gui() {
    ImGui::Begin("Transform Debugging");
    ImGui::SliderFloat3("position", (float*)&pos, 0.0f, 10.0f);
    ImGui::SliderFloat3("rotation", (float*)&rot, -2*glm::pi<float>(), 2*glm::pi<float>());
    ImGui::SliderFloat3("scale", (float*)&scale, 0.1f, 10.0f);
    ImGui::End();
  }
}
