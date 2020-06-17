#pragma once

#include "shader.hpp"

#include "../geometry/transform.hpp"
#include "../geometry/rectangle.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace open_pokemon_tcg::engine::graphics {

  class Rectangle {
  public:
    Rectangle(geometry::Rectangle rectangle);

    geometry::Transform transform;

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, Shader *shader);

  private:
    GLuint _vao;
  };

}
