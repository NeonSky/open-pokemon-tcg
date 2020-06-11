#pragma once

#include <glm/vec3.hpp>

namespace open_pokemon_tcg::collision_detection {

  struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
  };

}
