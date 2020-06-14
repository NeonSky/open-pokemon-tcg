#pragma once

#include "rectangle.hpp"

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

namespace open_pokemon_tcg::collision_detection {

  struct Plane {
    glm::vec3 point_on_plane;
    glm::vec3 normal;

    Plane() {}
    Plane(Rectangle rectangle) {
      this->point_on_plane = rectangle.botleft;
      this->normal = glm::cross(rectangle.botright - rectangle.botleft, rectangle.topleft - rectangle.botleft);
    }
  };

}
