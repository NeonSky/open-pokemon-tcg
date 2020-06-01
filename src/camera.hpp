#pragma once

#include "orientation.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace open_pokemon_tcg{

  // TODO: Move, remove or potentially change name. Maybe to player_camera
  enum Direction {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN,
  };

  class Camera {
  public:
    Camera();
    Camera(Orientation orientation);
    ~Camera();

    // Mutators
    void move(Direction dir);
    void look_at(glm::vec3 target);
    void lookat_mouse(float mouse_xpos, float mouse_ypos);

    // Accessors
    glm::mat4 view_matrix();

  private:
    glm::vec3 pos;
    Orientation orientation;
    float movement_speed = 0.5f;
    float rotation_speed = 0.1f;
  };

};
