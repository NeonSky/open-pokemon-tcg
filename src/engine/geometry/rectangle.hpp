#pragma once

#include "transform.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace open_pokemon_tcg::engine::geometry {

  struct Rectangle {
    glm::vec3 topleft;
    glm::vec3 botleft;
    glm::vec3 botright;

    Rectangle(Transform transform, float width, float height) {
      this->topleft  = transform.matrix() * glm::vec4(transform.position + glm::vec3(-width/2.0f, +height/2.0f, 0.0f), 1.0f);
      this->botleft  = transform.matrix() * glm::vec4(transform.position + glm::vec3(-width/2.0f, -height/2.0f, 0.0f), 1.0f);
      this->botright = transform.matrix() * glm::vec4(transform.position + glm::vec3(+width/2.0f, -height/2.0f, 0.0f), 1.0f);
    }

    Rectangle(glm::vec3 topleft, glm::vec3 botleft, glm::vec3 botright) : topleft(topleft), botleft(botleft), botright(botright) {}

    glm::vec3 topright() {
      return this->botleft + (this->topleft - this->botleft) + (this->botright - this->botleft);
    }
  };
}
