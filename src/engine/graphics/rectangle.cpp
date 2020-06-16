#include "rectangle.hpp"

#include <glm/vec3.hpp>

#include <vector>

using namespace open_pokemon_tcg::engine::graphics;

Rectangle::Rectangle(geometry::Rectangle rectangle) {
  glm::vec3 botleft  = rectangle.botleft;
  glm::vec3 botright = rectangle.botright;
  glm::vec3 topleft  = rectangle.topleft;
  glm::vec3 topright = rectangle.topright();

  std::vector<float> positions = {
    //	 X      Y     Z
      botleft.x , botleft.y , botleft.z,
      botright.x, botright.y, botright.z,
      topright.x, topright.y, topright.z,
      topleft.x , topleft.y , topleft.z,
  };

  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

  GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

  unsigned int index_buffer;
  const int indices[] = {
    0, 1, 2, // Triangle 1
    2, 3, 0  // Triangle 2
  };

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Assign position attribute of vertex shader
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

  this->vao = vao;
}

void Rectangle::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
  glm::mat4 modelViewProjectionMatrix = view_projection_matrix * this->transform.matrix();
  shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

  glBindVertexArray(this->vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
