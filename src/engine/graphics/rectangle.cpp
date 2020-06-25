#include "rectangle.hpp"

#include "../debug/logger.hpp"

#include <glm/vec3.hpp>

#include <vector>

using namespace open_pokemon_tcg::engine::graphics;

Rectangle::Rectangle(geometry::Rectangle rectangle, const Texture *texture) : _texture(texture){
  _shader = std::make_unique<Shader>("texture.vert", "texture.frag");

  glm::vec3 botleft  = rectangle.botleft();
  glm::vec3 botright = rectangle.botright();
  glm::vec3 topleft  = rectangle.topleft();
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

  unsigned int uv_buffer;
  if (_texture != nullptr) {
    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    const std::vector<float> uv_coords = {
      0.0f, 0.0f, // (u,v) for v0
      1.0f, 0.0f, // (u,v) for v1
      1.0f, 1.0f, // (u,v) for v2
      0.0f, 1.0f, // (u,v) for v3
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_coords.size(), uv_coords.data(), GL_STATIC_DRAW);
  }

  GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

  unsigned int index_buffer;
  const int indices[] = {
    0, 1, 2, // Triangle 1
    2, 3, 0, // Triangle 2
  };

	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Assign position attribute of vertex shader
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
  glEnableVertexAttribArray(0);

  if (_texture != nullptr) {
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);
  }

  _vao = vao;
}

void Rectangle::render(const glm::mat4 &view_projection_matrix) {
  _shader->use();

  glm::mat4 model_view_projection = view_projection_matrix * this->transform.matrix();
  _shader->set_uniform("mvp_matrix", &model_view_projection[0].x);
  // _shader->set_uniform("modelViewProjectionMatrix", &model_view_projection[0].x);

  if (_texture != nullptr) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture->id());
  }

  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
