#pragma once

#include "../engine/geometry/collision_detection.hpp"
#include "../engine/geometry/transform.hpp"
#include "../engine/graphics/shader.hpp"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace open_pokemon_tcg {
  class Card {
  public:
    engine::geometry::Transform transform;

    Card(engine::geometry::Transform transform, GLuint texture);
    ~Card();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader);

    // Accessors
    glm::mat4 model_matrix() const;
    engine::geometry::Intersection* does_intersect(engine::geometry::Ray ray);
    GLuint texture() const { return this->front_texture; }

  private:
    static constexpr float width = 1.0f;
    static constexpr float height = 1.0f;

    GLuint vao; // Vertex Array Object
    GLuint front_texture;
    GLuint back_texture;

    // Accessors
    GLuint create_vao(const std::vector<float> positions, const std::vector<float> uv_coords) const;
    engine::geometry::Rectangle shape() const;
    engine::geometry::Rectangle raw_shape() const;
  };
}
