#include "card.hpp"

#include "texture.hpp"
#include <vector>

using namespace open_pokemon_tcg;

GLuint my_create_vao(const std::vector<float> positions, const std::vector<float> uv_coords) {
  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

  const float colors[] = {
    //   R     G     B
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
  };

  unsigned int color_buffer;
  glGenBuffers(1, &color_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  unsigned int uv_buffer;
  glGenBuffers(1, &uv_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_coords.size(), uv_coords.data(), GL_STATIC_DRAW);

  GLuint vao; // Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

  // INDICES
  unsigned int index_buffer;
  const int indices[] = {
    0, 1, 2, // Triangle 1
    2, 3, 0  // Triangle 2
	};
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

	glEnableVertexAttribArray(0); // Enable the vertex position attribute
	glEnableVertexAttribArray(1); // Enable the vertex color attribute
	glEnableVertexAttribArray(2); // Enable the uv coord attribute

  return vao;
}

Card::Card(glm::vec3 pos, Orientation orientation, GLuint texture) : pos(pos), orientation(orientation), front_texture(texture) {

  float width = 1.0f;
  float height = 1.0f;
  glm::vec3 botleft = pos - (width/2.0f) * orientation.right() - (height/2.0f) * orientation.up();
  glm::vec3 botright = pos + (width/2.0f) * orientation.right() - (height/2.0f) * orientation.up();
  glm::vec3 topleft = pos - (width/2.0f) * orientation.right() + (height/2.0f) * orientation.up();
  glm::vec3 topright = pos + (width/2.0f) * orientation.right() + (height/2.0f) * orientation.up();

  GLuint vao = my_create_vao({
    //	 X      Y     Z
    botleft.x, botleft.y, botleft.z,
    botright.x, botright.y, botright.z,
    topright.x, topright.y, topright.z,
    topleft.x, topleft.y, topleft.z,
  }, {
    0.0f, 1.0f, // (u,v) for v0
    1.0f, 1.0f, // (u,v) for v1
    1.0f, 0.0f, // (u,v) for v2
    0.0f, 0.0f, // (u,v) for v3
  });
  this->front_vao = vao;

  vao = my_create_vao({
      //	 X      Y     Z
      topleft.x, topleft.y, topleft.z,
      topright.x, topright.y, topright.z,
      botright.x, botright.y, botright.z,
      botleft.x, botleft.y, botleft.z,
  }, {
      0.0f, 0.0f, // (u,v) for v0
      1.0f, 0.0f, // (u,v) for v1
      1.0f, 1.0f, // (u,v) for v2
      0.0f, 1.0f, // (u,v) for v3
  });
  this->back_vao = vao;

  this->back_texture = Texture("cardback.png").id();
}

Card::~Card() {}


void Card::render() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->front_texture);
  glBindVertexArray(this->front_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->back_texture);
  glBindVertexArray(this->back_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

glm::mat4 Card::model_matrix() {
  glm::mat4 m = glm::mat4(1.0f);
  m[3] = glm::vec4(this->pos, 1);
  return m;
}
