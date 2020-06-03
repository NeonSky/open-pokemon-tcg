#pragma once

#include "transform.hpp"

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

  enum ProjectionType {
    PERSPECTIVE,
    ORTHOGRAPHIC,
  };

  class Camera {
  public:
    Camera();
    Camera(Transform);
    ~Camera();

    // Mutators
    void move(Direction dir);
    void look_at(glm::vec3 target);
    void lookat_mouse(float mouse_xpos, float mouse_ypos);
    void set_zoom(float zoom_level);

    // Accessors
    glm::mat4 view_matrix() const;
    glm::mat4 projection_matrix(ProjectionType projection_type) const;

  private:
    struct Perspective {
      float fov;
      float aspect_ratio;
      float near;
      float far;

      Perspective() {
        fov = 45.0f;
        aspect_ratio = 16.0f / 9.0f;
        near = 0.1f;
        far = 300.0f;
      }
    };

    struct Orthographic {
      float left;
      float right;
      float bot;
      float top;
      float near;
      float far;

      Orthographic() {
        left = -1.0f;
        right = 1.0f;
        bot = -1.0f;
        top = 1.0f;
        near = 0.1f;
        far = 300.0f;
      }
    };

    Transform transform;
    Perspective perspective;
    Orthographic orthographic;
    float movement_speed = 0.5f;
    float rotation_speed = 0.1f;
  };

};
