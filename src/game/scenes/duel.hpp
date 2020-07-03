#pragma once

#include "debug_camera.hpp"
#include "../model/game.hpp"
#include "../data/pokemon_tcg_api.hpp"
#include "../view/card.hpp"
#include "../view/card_playmat.hpp"
#include "../view/deck.hpp"
#include "../view/game.hpp"
#include "../view/playmats/black_playmat.hpp"
#include "../view/playmats/green_playmat.hpp"

#include "../../engine/debug/debug_drawer.hpp"
#include "../../engine/debug/logger.hpp"
#include "../../engine/gui/window.hpp"
#include "../../engine/scene/scene.hpp"
#include "../../engine/graphics/rectangle.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

#include <array>

namespace open_pokemon_tcg::game::scenes {

  class Duel : public engine::scene::IScene {
  public:
    Duel(engine::gui::Window* window);
    ~Duel();

    // Mutators
    void update() override;
    void render() override;
    void gui() override;

  private:
    const engine::geometry::Transform camera1_transform = engine::geometry::Transform(glm::vec3(0.0f, 5.5f, -7.5f),
                                                 glm::vec3(-0.48f*glm::half_pi<float>(), 0.0f, 0.0f));
    const engine::geometry::Transform camera2_transform = engine::geometry::Transform(glm::vec3(0.0f, 5.5f, 7.5f),
                                                 glm::vec3(-0.48f*glm::half_pi<float>(), glm::pi<float>(), 0.0f));

    DebugCamera camera;
    engine::graphics::Shader *highlight_shader;
    view::IPlaymat *playmat;

    view::Player *current_player;
    view::Player *player1;
    view::Player *player2;

    view::Game *_game;

    view::Card *_selected_card;
    engine::graphics::Rectangle *debug_rect;
    bool focus_hovered_card = false;

    // Mutators
    void on_key(GLFWwindow* window);
    void on_mouse_click(GLFWwindow* window, int button, int action);
  };

  Duel::Duel(engine::gui::Window* window) : camera(DebugCamera(window, this->camera1_transform)) {
    _selected_card = nullptr;

    window->add_on_key_callback(std::bind(&Duel::on_key, this, std::placeholders::_1));
    window->add_on_mouse_click_callback(std::bind(&Duel::on_mouse_click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->highlight_shader = new engine::graphics::Shader("simple.vert", "highlight.frag");


    view::CardPlaymat playmat(std::make_unique<view::playmats::BlackPlaymat>());

    this->playmat = new view::playmats::BlackPlaymat();
    this->debug_rect = new engine::graphics::Rectangle(engine::geometry::Rectangle(engine::geometry::Transform()));

    LOG_DEBUG("Loading decks...");
    data::PokemonTcgApi api;
    std::array<std::unique_ptr<model::Deck>, 2> decks {
      api.load_deck("Base"),
      api.load_deck("Base"),
    };
    LOG_DEBUG("Decks loaded.");

    // TODO: Investigate why this takes so long
    std::array<std::string, 2> names {"Alice", "Bob"};
    std::shared_ptr<model::Game> game_model = std::make_shared<model::Game>(decks, names);
    _game = new view::Game(game_model, *this->playmat);

    this->player1 = _game->players()[0];
    this->player2 = _game->players()[1];
    this->current_player = this->player1;

    LOG_DEBUG("Scene loaded.");
  }

  Duel::~Duel() {}

  void Duel::update() {
    this->player1->update();
    this->player2->update();
  }

  void Duel::render() {
    glm::mat4 view_projection = this->camera.projection_matrix() * this->camera.view_matrix();

    _game->render(view_projection);

    engine::geometry::Ray ray;
    ray.origin = this->camera.transform().position;
    ray.direction = glm::normalize(this->camera.mouse_ray());

    // Check hover playmat
    engine::geometry::Rectangle *hit = nullptr;
    view::IPlaymat::Side hover_side = (this->current_player == this->player1) ? view::IPlaymat::Side::PLAYER1 : view::IPlaymat::Side::PLAYER2;
    auto slots = this->playmat->player_slots(hover_side);
    for (auto &slot : slots) {
      if (engine::geometry::ray_rectangle_intersection(ray, slot)) {
        hit = &slot;
        break;
      }
    }

    if (hit != nullptr) {
      this->debug_rect->transform.position = hit->transform().position;
      this->debug_rect->transform.rotation = hit->transform().rotation;
    } else {
      this->debug_rect->transform.position = glm::vec3(10000.0f, 0.0, 0.0f);
    }

    // Check hover cards
    view::Card *hover_card = nullptr;
    float best_dist = 1e9;

    for (auto &card : this->current_player->hand->cards()) {
      auto hit = card->does_intersect(ray);
      if (hit != nullptr) {
        float dist = glm::distance(hit->point, this->camera.transform().position);
        if (dist < best_dist) {
          best_dist = dist;
          hover_card = card;
        }
      }
    }

    for (auto card : this->current_player->discard_pile->cards()) {
      auto hit = card.does_intersect(ray);
      if (hit != nullptr) {
        float dist = glm::distance(hit->point, this->camera.transform().position);
        if (dist < best_dist) {
          best_dist = dist;
          hover_card = &card;
        }
      }
    }

    if (focus_hovered_card && hover_card != nullptr) {
      engine::geometry::Transform t = engine::geometry::Transform(this->camera.transform().position + 2.0f * this->camera.transform().forward());
      t.rotation = this->camera.transform().rotation;
      t.flip_rotation();

      engine::graphics::Rectangle detail(engine::geometry::Rectangle(t, 1.0f, 1.0f), &hover_card->texture());
      detail.render(view_projection);
    }

    this->highlight_shader->use();
    this->debug_rect->render(view_projection);
  }

  void Duel::gui() {
    ImGui::Begin("Statistics");
    ImGui::Text("Turn: %d", _game->model().turn());
    ImGui::Text("Player: %s", _game->model().current_player().name().c_str());
    ImGui::End();

    ImGui::Begin("Player Controls");

    if (ImGui::Button("End Turn")) {
      _game->model().end_turn();
      this->current_player = (this->current_player == this->player1) ? this->player2 : player1; // TODO: Maybe move to view?
      this->camera.set_transform((this->current_player == this->player1) ? this->camera1_transform : this->camera2_transform);

      // if (_game->model().winner() != nullptr) {
      //   LOG_DEBUG("We have a winner!");
      //   LOG_DEBUG("The winner is: " + _game->model().winner()->name());
      //   return;
      // }
    }

    if (_selected_card != nullptr) {
      ImGui::Separator();

      ImGui::Text("Selected Card Options");
      if (ImGui::Button("Place on active slot")) {
        _game->model().current_player().active_pokemon_from_hand(_selected_card->_model);
        _selected_card = nullptr;
      }
      if (ImGui::Button("Place on bench")) {
        _game->model().current_player().bench_pokemon_from_hand(_selected_card->_model);
        _selected_card = nullptr;
      }

      ImGui::Separator();
    }
    ImGui::End();

    ImGui::Begin("Debug Controls");
    if (ImGui::Button("Draw Card"))
      _game->model().current_player().draw();
    if (ImGui::Button("Mill Card"))
      _game->model().current_player().mill();
    if (ImGui::Button("Take Prize Card"))
      _game->model().current_player().take_prize_card();
    ImGui::End();
  }

  void Duel::on_key(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
      _selected_card = nullptr;

    // Switch which user/player to control
    if (glfwGetKey(window, GLFW_KEY_U)) {
      _game->model().end_turn();

      if (_game->model().winner() != nullptr) {
        LOG_DEBUG("We have a winner!");
        LOG_DEBUG("The winner is: " + _game->model().winner()->name());
        return;
      }

      this->current_player = (this->current_player == this->player1) ? this->player2 : player1;
      this->camera.set_transform((this->current_player == this->player1) ? this->camera1_transform : this->camera2_transform);
    }

    focus_hovered_card = glfwGetKey(window, GLFW_KEY_F);
  }

  void Duel::on_mouse_click([[maybe_unused]] GLFWwindow* window, int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT  && action == GLFW_PRESS) {

      engine::geometry::Ray ray;
      ray.origin = this->camera.transform().position;
      ray.direction = glm::normalize(this->camera.mouse_ray());

      view::IPlaymat::Side current_side = (this->current_player == this->player1) ? view::IPlaymat::Side::PLAYER1 : view::IPlaymat::Side::PLAYER2;

      if (_selected_card != nullptr) {
        if (engine::geometry::ray_rectangle_intersection(ray, this->playmat->active_slot(current_side))) {
          _game->model().current_player().active_pokemon_from_hand(_selected_card->_model);
          _selected_card = nullptr;
        }
      }

      // _selected_card = nullptr;
      float best_dist = 1e9;

      for (auto &card : this->current_player->hand->cards()) {
        auto hit = card->does_intersect(ray);
        if (hit != nullptr) {
          float dist = glm::distance(hit->point, this->camera.transform().position);
          if (dist < best_dist) {
            best_dist = dist;
            _selected_card = card;
          }
        }
      }
    }
  }
}
