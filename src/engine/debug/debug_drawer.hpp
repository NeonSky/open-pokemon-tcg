#pragma once

#include "../geometry/rectangle.hpp"
#include "../geometry/transform.hpp"
#include "../graphics/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace open_pokemon_tcg::engine::debug {

  struct Line {
    glm::vec3 A, B;
    glm::vec3 color;

    Line(glm::vec3 A, glm::vec3 B) : A(A), B(B), color(glm::vec3(1.0f, 0.0f, 0.0f)) {}
    Line(glm::vec3 A, glm::vec3 B, glm::vec3 color) : A(A), B(B), color(color) {}
  };

  class DebugDrawer {
  public:
    DebugDrawer();
    ~DebugDrawer();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix);
    void draw_line(glm::vec3 from, glm::vec3 to);
    void draw_transform(geometry::Transform transform);
    void draw_rectangle(geometry::Rectangle rectangle);

  private:
    const float line_width = 5.0f;

    graphics::Shader shader;

    std::vector<Line> line_queue;
  };

}
