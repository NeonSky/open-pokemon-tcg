#pragma once

#include "../scene.hpp"
#include "../card.hpp"
#include "../texture.hpp"
#include "../window.hpp"
#include "../debug_camera.hpp"
#include "../pokemon_tcg_api.hpp"
#include "../playmats/black_playmat.hpp"
#include "../logger.hpp"
#include "../duel_player.hpp"
#include "../deck.hpp"

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
    const Transform camera1_transform = Transform(glm::vec3(0.0f, 5.5f, -7.5f),
                                                 glm::vec3(-0.48f*glm::half_pi<float>(), 0.0f, 0.0f));
    const Transform camera2_transform = Transform(glm::vec3(0.0f, 5.5f, 7.5f),
                                                 glm::vec3(-0.48f*glm::half_pi<float>(), glm::pi<float>(), 0.0f));

    DebugCamera camera;
    Shader *shader;
    IPlaymat *playmat;

    DuelPlayer *current_player;
    DuelPlayer *player1;
    DuelPlayer *player2;

    bool focus_hovered_card = false;

    // Mutators
    void on_key(GLFWwindow* window);
  };

  Duel::Duel(Window* window) : camera(DebugCamera(window, this->camera1_transform)) {

    window->add_on_key_callback(std::bind(&Duel::on_key, this, std::placeholders::_1));
    this->shader = new Shader("simple.vert", "simple.frag");

    this->playmat = new playmats::BlackPlaymat();

    PokemonTcgApi *api = new PokemonTcgApi();

    LOG_DEBUG("Loading deck1...");
    nlohmann::json deck1_data = api->load_deck("Base").data;
    Deck deck1;
    for (auto &card : deck1_data[0]["cards"])
      for (int i = 0; i < card["count"]; i++)
        deck1.cards.push_back(Card(Transform(), api->load_card((std::string)card["id"]).texture.id()));

    LOG_DEBUG("Loading deck2...");
    nlohmann::json deck2_data = api->load_deck("Jungle").data;
    Deck deck2;
    for (auto &card : deck2_data[0]["cards"])
      for (int i = 0; i < card["count"]; i++)
        deck2.cards.push_back(Card(Transform(), api->load_card((std::string)card["id"]).texture.id()));

    this->player1 = new DuelPlayer(deck1, *this->playmat, IPlaymat::Side::PLAYER1);
    this->player2 = new DuelPlayer(deck2, *this->playmat, IPlaymat::Side::PLAYER2);
    this->current_player = this->player1;

    LOG_DEBUG("Scene loaded.");
  }

  Duel::~Duel() {}

  void Duel::update() {
    this->player1->update();
    this->player2->update();
  }

  void Duel::render() {
    this->shader->use();

    glm::mat4 viewMatrix = this->camera.view_matrix();
    glm::mat4 projectionMatrix = this->camera.projection_matrix();
    glm::mat4 view_projection_matrix = projectionMatrix * viewMatrix;

    this->player1->render(view_projection_matrix, this->shader);
    this->player2->render(view_projection_matrix, this->shader);

    this->playmat->render(view_projection_matrix, this->shader);

    collision_detection::Ray ray;
    ray.origin = this->camera.transform().position;
    ray.direction = glm::normalize(this->camera.mouse_ray());

    // Check hover playmat
    auto in = this->playmat->does_intersect(ray);
    if (in != nullptr) {
      LOG_DEBUG(in->side);
      LOG_DEBUG(in->area_type);
    }

    // Check hover cards
    Card *hover_card = nullptr;
    float best_dist = 1e9;

    for (auto &card : this->current_player->hand->cards) {
      auto hit = card->does_intersect(ray);
      if (hit != nullptr) {
        float dist = glm::distance(hit->point, this->camera.transform().position);
        if (dist < best_dist) {
          best_dist = dist;
          hover_card = card;
        }
      }
    }

    for (auto &card : this->current_player->discard_pile->cards) {
      auto hit = card->does_intersect(ray);
      if (hit != nullptr) {
        float dist = glm::distance(hit->point, this->camera.transform().position);
        if (dist < best_dist) {
          best_dist = dist;
          hover_card = card;
        }
      }
    }

    if (focus_hovered_card && hover_card != nullptr) {
      Transform t = Transform(this->camera.transform().position + 2.0f * this->camera.transform().forward());
      t.rotation = glm::vec3(0.5f*glm::half_pi<float>(), this->camera.transform().rotation.y, 0.0f);
      Card detail = Card(t, hover_card->texture());
      detail.render(view_projection_matrix, this->shader);
    }
  }

  void Duel::gui() {}

  void Duel::on_key(GLFWwindow* window) {
    // Mill
    if (glfwGetKey(window, GLFW_KEY_M)) {
      this->current_player->mill();
    }

    // Hand
    if (glfwGetKey(window, GLFW_KEY_H)) {
      this->current_player->hand->cards.push_back(this->current_player->deck_pile->draw());
    }

    // Switch which user/player to control
    if (glfwGetKey(window, GLFW_KEY_U))
      this->current_player = (this->current_player == this->player1) ? this->player2 : player1;

    if (glfwGetKey(window, GLFW_KEY_1))
      this->camera.set_transform(this->camera1_transform);

    if (glfwGetKey(window, GLFW_KEY_2))
      this->camera.set_transform(this->camera2_transform);

    focus_hovered_card = glfwGetKey(window, GLFW_KEY_F);
  }
}
