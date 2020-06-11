#pragma once

#include <glm/vec3.hpp>

namespace open_pokemon_tcg::collision_detection {

  struct Rectangle {
    glm::vec3 topleft;
    glm::vec3 botleft;
    glm::vec3 botright;

    Rectangle(glm::vec3 topleft, glm::vec3 botleft, glm::vec3 botright) : topleft(topleft), botleft(botleft), botright(botright) {}
  };

}
