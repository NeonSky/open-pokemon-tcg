#pragma once

#include "transform.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace open_pokemon_tcg::engine::geometry {

  class Rectangle {
  public:
    Rectangle();
    Rectangle(Transform transform, float width = 1.0f, float height = 1.0f);

    // Accessors
    Transform transform() const;
    float width() const;
    float height() const;

    glm::vec3 topleft() const;
    glm::vec3 topright() const;
    glm::vec3 botleft() const;
    glm::vec3 botright() const;

  private:
    Transform _transform;
    float _width;
    float _height;

    glm::vec3 _topleft;
    glm::vec3 _topright;
    glm::vec3 _botleft;
    glm::vec3 _botright;

    // Mutators
    void update_corners();

  };
}
