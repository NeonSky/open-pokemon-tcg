#pragma once

#include "../../card.hpp"
#include "../../debug_camera.hpp"

#include "../../../engine/debug/logger.hpp"
#include "../../../engine/graphics/texture.hpp"
#include "../../../engine/gui/window.hpp"
#include "../../../engine/scene/scene.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

namespace open_pokemon_tcg::scenes {

class CardTransform : public engine::scene::IScene {
  public:
    CardTransform(engine::gui::Window* window);
    ~CardTransform();

    void update() override;
    void render() override;
    void gui() override;

  private:
    DebugCamera camera;
    Card *debug_card;
    engine::graphics::Shader *shader;

    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 rot = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
  };

  CardTransform::CardTransform(engine::gui::Window* window) :
    camera(DebugCamera(window,
                       engine::geometry::Transform(glm::vec3(0.0f, 2.0f, 0.0f),
                                 glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)))) {

    this->shader = new engine::graphics::Shader("simple.vert", "simple.frag");
    debug_card = new Card(engine::geometry::Transform(), engine::graphics::Texture("cache/cards/img/base1-26.png").id());

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
