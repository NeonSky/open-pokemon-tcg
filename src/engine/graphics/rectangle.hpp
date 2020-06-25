#pragma once

#include "shader.hpp"
#include "texture.hpp"

#include "../geometry/transform.hpp"
#include "../geometry/rectangle.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace open_pokemon_tcg::engine::graphics {

  class Rectangle {
  public:
    Rectangle(geometry::Rectangle rectangle, const Texture *texture = nullptr);

    geometry::Transform transform;

    // Mutators
    void render(const glm::mat4 &view_projection_matrix);

  private:
    GLuint _vao;
    const Texture *_texture;
    std::unique_ptr<Shader> _shader;
  };

}
