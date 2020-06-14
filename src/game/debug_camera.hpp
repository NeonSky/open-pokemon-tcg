#pragma once

#include "../engine/graphics/camera.hpp"
#include "../engine/gui/window.hpp"

namespace open_pokemon_tcg {

  class DebugCamera {
  public:
    DebugCamera(Window* window);
    DebugCamera(Window* window, Transform transform);
    ~DebugCamera();

    // Mutators
    void set_transform(Transform transform);

    // Accessors
    Transform transform() const { return this->camera.transform; };
    glm::mat4 view_matrix() const { return this->camera.view_matrix(); };
    glm::mat4 projection_matrix() const {return this->camera.projection_matrix(this->projection_type); };
    glm::vec3 mouse_ray() const { return this->_mouse_ray; }

  private:
    static constexpr float zoom_min = 0.0f;
    static constexpr float zoom_max = 45.0f;

    Camera camera;
    Window* window;

    glm::vec3 _mouse_ray;
    ProjectionType projection_type = ProjectionType::PERSPECTIVE;
    bool free_look_mode = false;
    float zoom = 0.0f;

    // Mutators
    void on_key(GLFWwindow* window);
    void on_cursor(GLFWwindow* window, float xpos, float ypos);
    void on_scroll(GLFWwindow* window, float xoffset, float yoffset);
  };
}
