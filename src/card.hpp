#pragma once

#include "orientation.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace open_pokemon_tcg{
  class Card {
  public:
    Card(glm::vec3 pos, Orientation orientation, GLuint texture);
    ~Card();
    void render();

    // Accessors
    static constexpr glm::mat4 model_matrix = glm::mat4(1.0f); // TODO: make this individual
  private:
    glm::vec3 pos;
    Orientation orientation;

    GLuint front_texture;
    GLuint front_vao;
    GLuint back_texture;
    GLuint back_vao;
  };
}
