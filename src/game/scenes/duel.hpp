#pragma once

#include "debug_camera.hpp"
#include "../model/game.hpp"
#include "../model/effects/card_effect.hpp"
#include "../data/pokemon_tcg_api.hpp"
#include "../view/card.hpp"
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
    view::Player *next_player;
    view::Player *player1;
    view::Player *player2;

    view::Game *_game;
    const model::ICardEffect* _activated_effect;
    std::vector<std::reference_wrapper<const model::ICard>> _activated_effect_targets;

    view::Card *_selected_card;
    engine::graphics::Rectangle *debug_rect;
    bool focus_hovered_card = false;

    // Mutators
    void on_key(GLFWwindow* window);
    void on_mouse_click(GLFWwindow* window, int button, int action);
  };

  Duel::Duel(engine::gui::Window* window) : camera(DebugCamera(window, this->camera1_transform)) {
    _selected_card = nullptr;
    _activated_effect = nullptr;

    window->add_on_key_callback(std::bind(&Duel::on_key, this, std::placeholders::_1));
    window->add_on_mouse_click_callback(std::bind(&Duel::on_mouse_click, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    this->highlight_shader = new engine::graphics::Shader("simple.vert", "highlight.frag");

    this->playmat = new view::playmats::BlackPlaymat();
    this->debug_rect = new engine::graphics::Rectangle(engine::geometry::Rectangle(engine::geometry::Transform()));

    LOG_DEBUG("Loading decks...");
    data::PokemonTcgApi api;
    std::array<std::unique_ptr<model::Deck>, 2> decks {
      api.load_deck("Base"),
      api.load_deck("Base"),
    };
    LOG_DEBUG("Decks loaded.");

    std::array<std::string, 2> names {"Alice", "Bob"};
    std::shared_ptr<model::Game> game_model = std::make_shared<model::Game>(decks, names);
    game_model->on_game_over([this]() {
      LOG_DEBUG("We have a winner!");
      LOG_DEBUG("The winner is: " + _game->model().winner()->name());
    });

    _game = new view::Game(game_model, *this->playmat);

    this->player1 = _game->players()[0];
    this->player2 = _game->players()[1];
    this->current_player = this->player1;
    this->next_player = this->player2;

    LOG_DEBUG("Scene loaded.");
  }

  Duel::~Duel() {}

  void Duel::update() {
    if (_activated_effect != nullptr && _activated_effect_targets.size() == _activated_effect->required_targets().size()) {
      LOG_DEBUG("Targets fulfilled! Activating card effect...");
      _game->model().activate_trainer_card(_selected_card->_model, _activated_effect_targets);
      _activated_effect = nullptr;
      _activated_effect_targets.clear();
    }

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
      if (_game->model().winner() == nullptr) {
        _game->model().end_turn();
        std::swap(current_player, next_player);
        this->camera.set_transform((this->current_player == this->player1) ? this->camera1_transform : this->camera2_transform);
      }
    }

    if (_selected_card != nullptr) {
      ImGui::Separator();

      ImGui::Text("Selected Card Options");
      if (ImGui::Button("Place on active slot")) {
        _game->model().place_on_active_slot_from_hand(_selected_card->_model);
        _selected_card = nullptr;
      }
      if (ImGui::Button("Place on bench")) {
        _game->model().place_on_bench_from_hand(_selected_card->_model);
        _selected_card = nullptr;
      }
      if (ImGui::Button("Activate card")) {

        _activated_effect = &(dynamic_cast<const model::ItemCard*>(&_selected_card->_model)->effect());
        if (_activated_effect == nullptr)
          LOG_WARNING("Card must be an item card.");

        if (_activated_effect->required_targets().size() == 0) {
          _game->model().activate_trainer_card(_selected_card->_model, {});
          _selected_card = nullptr;
          _activated_effect = nullptr;
        }
        else {
          _activated_effect_targets.clear();
        }
      }

      ImGui::Separator();
    }
    ImGui::End();

    ImGui::Begin("Debug Controls");
    // if (ImGui::Button("Draw Card"))
    //   _game->model().current_player().draw();
    // if (ImGui::Button("Mill Card"))
    //   _game->model().current_player().mill();
    // if (ImGui::Button("Take Prize Card"))
    //   _game->model().current_player().take_prize_card();
    ImGui::End();
  }

  void Duel::on_key(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
      _selected_card = nullptr;

    focus_hovered_card = glfwGetKey(window, GLFW_KEY_F);
  }

  void Duel::on_mouse_click([[maybe_unused]] GLFWwindow* window, int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT  && action == GLFW_PRESS) {

      engine::geometry::Ray ray;
      ray.origin = this->camera.transform().position;
      ray.direction = glm::normalize(this->camera.mouse_ray());

      view::IPlaymat::Side current_side = (this->current_player == this->player1) ? view::IPlaymat::Side::PLAYER1 : view::IPlaymat::Side::PLAYER2;
      view::IPlaymat::Side opponent_side = (this->current_player == this->player1) ? view::IPlaymat::Side::PLAYER2 : view::IPlaymat::Side::PLAYER1;

      if (_activated_effect != nullptr && _activated_effect_targets.size() < _activated_effect->required_targets().size()) {

        int i = _activated_effect_targets.size();
        auto next_target = _activated_effect->required_targets()[i];

        if (next_target == model::CardEffectTarget::ENEMY_POKEMON) {

          // Check active
          if (_game->model().next_player().playmat().active_pokemon != nullptr && engine::geometry::ray_rectangle_intersection(ray, this->playmat->active_slot(opponent_side)))
            _activated_effect_targets.push_back(*_game->model().next_player().playmat().active_pokemon);

          // Check bench
          else {
            int i = 0;
            for (auto& slot : playmat->bench_slots(opponent_side)) {
              if (_game->model().next_player().playmat().bench->cards()[i] != nullptr && engine::geometry::ray_rectangle_intersection(ray, slot)) {
                _activated_effect_targets.push_back(*_game->model().next_player().playmat().bench->cards()[i]);
                break;
              }
              i++;
            }
          }
        }
        else if (next_target == model::CardEffectTarget::ENERGY_FROM_PREVIOUS_POKEMON) {

          auto prev = _activated_effect_targets[i-1];

          // Check active
          if (&prev.get() == _game->model().next_player().playmat().active_pokemon) {
            for (auto& energy_view : next_player->active_pokemon->energy_views()) {
              if (engine::geometry::ray_rectangle_intersection(ray, energy_view->shape())) {
                _activated_effect_targets.push_back(energy_view->_model);
                break;
              }
            }
          }

          // // Check bench
          else {
            int i = 0;
            for (auto& pokemon : _game->model().next_player().playmat().bench->cards()) {
              if (&prev.get() == pokemon) {
                for (auto& energy_view : next_player->bench->cards()[i]->energy_views()) {
                  if (engine::geometry::ray_rectangle_intersection(ray, energy_view->shape())) {
                    _activated_effect_targets.push_back(energy_view->_model);
                    break;
                  }
                }
              }
              i++;
            }
          }
        }

        return;
      }

      if (_selected_card != nullptr) {
        if (engine::geometry::ray_rectangle_intersection(ray, this->playmat->active_slot(current_side))) {

          if (_game->model().current_player().playmat().active_pokemon == nullptr)
            _game->model().place_on_active_slot_from_hand(_selected_card->_model);
          else
            _game->model().attach_to_active_pokemon(_selected_card->_model);

          _selected_card = nullptr;
          return;
        }

        auto slots = playmat->bench_slots(current_side);
        for (auto &slot : slots) {
          if (engine::geometry::ray_rectangle_intersection(ray, slot)) {

            int index = -1;
            for (unsigned int i = 0; i < slots.size(); i++) {
              if (&slot == &slots[i]) {
                index = i;
                break;
              }
            }

            if (index == -1)
              LOG_ERROR("Didn't figure out what bench slot was clicked on. This should never happen.");

            if (_game->model().current_player().playmat().bench->cards()[index] == nullptr)
              _game->model().place_on_bench_from_hand(_selected_card->_model, index);
            else
              _game->model().attach_to_bench_pokemon(_selected_card->_model, index);

            _selected_card = nullptr;
          }
        }

        if (engine::geometry::ray_rectangle_intersection(ray, this->playmat->active_slot(current_side))) {
        }
      }

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
