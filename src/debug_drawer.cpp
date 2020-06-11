#include "debug_drawer.hpp"
#include "logger.hpp"

using namespace open_pokemon_tcg;

DebugDrawer::DebugDrawer() : shader(Shader("projection_only.vert", "red.frag")) {}
DebugDrawer::~DebugDrawer() {}

void DebugDrawer::render(const glm::mat4 &view_projection_matrix) {

  // Render lines
  this->shader.use();
  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Line) * this->line_queue.size(), this->line_queue.data(), GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(vao);

  this->shader.set_uniform("modelViewProjectionMatrix", &view_projection_matrix[0].x);

  glLineWidth(this->line_width);
  glDrawArrays(GL_LINES, 0, 6 * this->line_queue.size()); // NOTE: 6 floats per line
  this->line_queue.clear();
}

void DebugDrawer::draw_line(glm::vec3 from, glm::vec3 to) {
  this->line_queue.push_back(Line(from, to));
}
