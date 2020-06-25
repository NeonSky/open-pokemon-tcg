#pragma once

#include "../../engine/graphics/camera.hpp"
#include "../../engine/gui/window.hpp"

namespace open_pokemon_tcg::game::scenes {

  class DebugCamera {
  public:
    DebugCamera(engine::gui::Window* window);
    DebugCamera(engine::gui::Window* window, engine::geometry::Transform transform);
    ~DebugCamera();

    // Mutators
    void set_transform(engine::geometry::Transform transform);

    // Accessors
    engine::geometry::Transform transform() const { return this->camera.transform; };
    glm::mat4 view_matrix() const { return this->camera.view_matrix(); };
    glm::mat4 projection_matrix() const {return this->camera.projection_matrix(this->projection_type); };
    glm::vec3 mouse_ray() const { return this->_mouse_ray; }

  private:
    static constexpr float zoom_min = 0.0f;
    static constexpr float zoom_max = 45.0f;

    engine::graphics::Camera camera;
    engine::gui::Window* window;

    glm::vec3 _mouse_ray;
    engine::graphics::ProjectionType projection_type = engine::graphics::ProjectionType::PERSPECTIVE;
    bool free_look_mode = false;
    float zoom = 0.0f;

    // Mutators
    void on_key(GLFWwindow* window);
    void on_cursor(GLFWwindow* window, float xpos, float ypos);
    void on_scroll(GLFWwindow* window, float xoffset, float yoffset);
  };
}
