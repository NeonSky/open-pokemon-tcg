#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace open_pokemon_tcg::engine::geometry {

  class Transform {
  public:
    Transform();
    Transform(glm::vec3 position);
    Transform(glm::vec3 position, glm::vec3 rotation);
    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    ~Transform();

    // Mutators
    void set_rotation(float yaw, float pitch, float roll);
    void flip_rotation();

    // Accessors
    static constexpr glm::vec4 world_right   = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    static constexpr glm::vec4 world_up      = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    static constexpr glm::vec4 world_forward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    Transform operator+(const Transform &other) const;

    glm::mat4 matrix() const;
    glm::vec3 forward() const;
    glm::vec3 up() const;
    glm::vec3 right() const;

    float yaw() const { return rotation.y; };
    float pitch() const { return rotation.x; };
    float roll() const { return rotation.z; };

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

  private:
    // Accessors
    glm::mat4 rotation_matrix() const;
  };
}
