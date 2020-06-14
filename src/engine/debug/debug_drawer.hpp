#pragma once

#include "../graphics/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace open_pokemon_tcg {

  struct Line {
    glm::vec3 A, B;
    Line(glm::vec3 A, glm::vec3 B) : A(A), B(B) {}
  };

  class DebugDrawer {
  public:
    DebugDrawer();
    ~DebugDrawer();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix);
    void draw_line(glm::vec3 from, glm::vec3 to);

  private:
    const float line_width = 5.0f;

    Shader shader;

    std::vector<Line> line_queue;
  };

}
