#pragma once

#include <glm/vec3.hpp>

namespace open_pokemon_tcg{

  class Orientation {
  public:
    Orientation();
    Orientation(glm::vec3 forward);
    ~Orientation();

    glm::vec3 forward() const { return _forward; }
    glm::vec3 right() const { return _right; }
    glm::vec3 up() const { return _up; }

  private:
    static constexpr glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 _forward;
    glm::vec3 _right;
    glm::vec3 _up;
  };

}
