#pragma once

#include "transform.hpp"
#include "shader.hpp"

#include <vector>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace open_pokemon_tcg{
  class Card {
  public:
    Transform transform;

    Card(Transform transform, GLuint texture);
    ~Card();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, Shader *shader);

    // Accessors
    glm::mat4 model_matrix() const;
  private:
    GLuint vao; // Vertex Array Object
    GLuint front_texture;
    GLuint back_texture;

    // Accessors
    GLuint create_vao(const std::vector<float> positions, const std::vector<float> uv_coords) const;
  };
}
