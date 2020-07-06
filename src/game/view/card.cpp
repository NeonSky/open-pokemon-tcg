#include "card.hpp"

#include "../../engine/debug/logger.hpp"
#include "../../engine/debug/debug_drawer.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

using namespace open_pokemon_tcg;
using namespace open_pokemon_tcg::game::view;

Card::Card(const game::model::ICard &model, engine::geometry::Transform transform)
  : _model(model),
    transform(transform),
    front_texture(engine::graphics::Texture("cache/cards/img/" + _model.id() + ".png")),
    back_texture(engine::graphics::Texture("img/cardback.png")) {

  engine::geometry::Rectangle rect(engine::geometry::Transform(), this->width, this->height);
  glm::vec3 botleft  = rect.botleft();
  glm::vec3 botright = rect.botright();
  glm::vec3 topleft  = rect.topleft();
  glm::vec3 topright = rect.topright();

  this->vao = create_vao({
    //	 X      Y     Z
    botleft.x , botleft.y , botleft.z,
    botright.x, botright.y, botright.z,
    topright.x, topright.y, topright.z,
    topleft.x , topleft.y , topleft.z,
  }, {
      0.0f, 0.0f, // (u,v) for v0
      1.0f, 0.0f, // (u,v) for v1
      1.0f, 1.0f, // (u,v) for v2
      0.0f, 1.0f, // (u,v) for v3
  });
}

Card::~Card() {}

void Card::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  shader->use();

  glm::mat4 front_matrix = this->transform.matrix();
  glm::mat4 modelViewProjectionMatrix = view_projection_matrix * front_matrix;
  shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->front_texture.id());
  glBindVertexArray(this->vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glm::mat4 back_matrix = glm::rotate(front_matrix, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
  modelViewProjectionMatrix = view_projection_matrix * back_matrix;
  shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

  glBindTexture(GL_TEXTURE_2D, this->back_texture.id());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

engine::geometry::Intersection* Card::does_intersect(engine::geometry::Ray ray) const {
  return engine::geometry::ray_rectangle_intersection(ray, shape());
}

const engine::graphics::Texture& Card::texture() const {
  return this->front_texture;
}

engine::geometry::Rectangle Card::shape() const {
  return engine::geometry::Rectangle(transform, width, height);
}

GLuint Card::create_vao(const std::vector<float> positions, const std::vector<float> uv_coords) const {
  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

  unsigned int uv_buffer;
  glGenBuffers(1, &uv_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_coords.size(), uv_coords.data(), GL_STATIC_DRAW);

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

  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

	glEnableVertexAttribArray(0); // Enable the vertex position attribute
	glEnableVertexAttribArray(2); // Enable the uv coord attribute

  return vao;
}
