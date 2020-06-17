#pragma once

#include "rectangle.hpp"

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

namespace open_pokemon_tcg::engine::geometry {

  struct Plane {
    glm::vec3 point_on_plane;
    glm::vec3 normal;

    Plane() {}
    Plane(Rectangle rectangle) {
      this->point_on_plane = rectangle.botleft();
      this->normal = rectangle.transform().forward();
    }
  };

}
