#pragma once

#include "camera.hpp"
#include "window.hpp"

namespace open_pokemon_tcg {

  class DebugCamera {
  public:
    DebugCamera(Window* window);
    DebugCamera(Window* window, Transform transform);
    ~DebugCamera();

    // Accessors
    glm::mat4 view_matrix() const { return this->camera.view_matrix(); };
    glm::mat4 projection_matrix() const {return this->camera.projection_matrix(this->projection_type); };

  private:
    static constexpr float zoom_min = 0.0f;
    static constexpr float zoom_max = 45.0f;

    Camera camera;
    Window* window;

    ProjectionType projection_type = ProjectionType::PERSPECTIVE;
    bool free_look_mode = false;
    float zoom = 0.0f;

    // Mutators
    void on_key(GLFWwindow* window);
    void on_cursor(GLFWwindow* window, float xpos, float ypos);
    void on_scroll(GLFWwindow* window, float xoffset, float yoffset);
  };
}