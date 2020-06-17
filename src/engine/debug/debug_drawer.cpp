#include "debug_drawer.hpp"

#include "logger.hpp"

using namespace open_pokemon_tcg::engine::debug;

DebugDrawer::DebugDrawer() : shader(graphics::Shader("color.vert", "color.frag")) {}
DebugDrawer::~DebugDrawer() = default;

void DebugDrawer::render(const glm::mat4 &view_projection_matrix) {

  this->shader.use();

  std::vector<float> positions;
  for (auto &line : this->line_queue) {
    positions.emplace_back(line.A.x);
    positions.emplace_back(line.A.y);
    positions.emplace_back(line.A.z);
    positions.emplace_back(line.B.x);
    positions.emplace_back(line.B.y);
    positions.emplace_back(line.B.z);
  }

  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

  std::vector<float> colors;
  for (auto &line : this->line_queue) {
    for (int i = 0; i < 2; i++) {
      colors.emplace_back(line.color.x);
      colors.emplace_back(line.color.y);
      colors.emplace_back(line.color.z);
    }
  }

  unsigned int color_buffer;
  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.data(), GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(vao);

  this->shader.set_uniform("modelViewProjectionMatrix", &view_projection_matrix[0].x);

  glLineWidth(this->line_width);
  glDrawArrays(GL_LINES, 0, 6 * this->line_queue.size()); // NOTE: 6 floats per line
  this->line_queue.clear();
}

void DebugDrawer::draw_line(glm::vec3 from, glm::vec3 to) {
  this->line_queue.emplace_back(from, to);
}

void DebugDrawer::draw_transform(geometry::Transform transform) {
  this->line_queue.emplace_back(transform.position, transform.position + transform.right()  , glm::vec3(1.0f, 0.0f, 0.0f));
  this->line_queue.emplace_back(transform.position, transform.position + transform.up()     , glm::vec3(0.0f, 1.0f, 0.0f));
  this->line_queue.emplace_back(transform.position, transform.position + transform.forward(), glm::vec3(0.0f, 0.0f, 1.0f));
}

void DebugDrawer::draw_rectangle(geometry::Rectangle rectangle) {
  this->line_queue.emplace_back(rectangle.topleft(), rectangle.topright());
  this->line_queue.emplace_back(rectangle.topleft(), rectangle.botleft());
  this->line_queue.emplace_back(rectangle.botright(), rectangle.botleft());
  this->line_queue.emplace_back(rectangle.botright(), rectangle.topright());
}
