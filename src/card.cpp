#include "card.hpp"
#include "texture.hpp"
#include "logger.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace open_pokemon_tcg;

Card::Card(Transform transform, GLuint texture) : transform(transform), front_texture(texture) {
  collision_detection::Rectangle rect = raw_shape();
  glm::vec3 botleft  = rect.botleft;
  glm::vec3 botright = rect.botright;
  glm::vec3 topleft  = rect.topleft;
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

  this->back_texture = Texture("img/cardback.png").id();
}

Card::~Card() {}

void Card::render(const glm::mat4 &view_projection_matrix, Shader *shader) {

  glm::mat4 front_matrix = this->model_matrix();
  glm::mat4 modelViewProjectionMatrix = view_projection_matrix * front_matrix;
  shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->front_texture);
  glBindVertexArray(this->vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glm::mat4 back_matrix = glm::rotate(front_matrix, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
  modelViewProjectionMatrix = view_projection_matrix * back_matrix;
  shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

  glBindTexture(GL_TEXTURE_2D, this->back_texture);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

glm::mat4 Card::model_matrix() const {
  glm::mat4 m = this->transform.matrix();
  m = glm::rotate(m, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
  return m;
}

collision_detection::Intersection* Card::does_intersect(collision_detection::Ray ray) {
  return collision_detection::ray_rectangle_intersection(ray, shape());
}

GLuint Card::create_vao(const std::vector<float> positions, const std::vector<float> uv_coords) const {
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

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

	glEnableVertexAttribArray(0); // Enable the vertex position attribute
	glEnableVertexAttribArray(1); // Enable the vertex color attribute
	glEnableVertexAttribArray(2); // Enable the uv coord attribute

  return vao;
}

collision_detection::Rectangle Card::shape() const {
  collision_detection::Rectangle rect = raw_shape();
  glm::vec3 topleft  = this->model_matrix() * glm::vec4(rect.topleft , 1.0f);
  glm::vec3 botleft  = this->model_matrix() * glm::vec4(rect.botleft , 1.0f);
  glm::vec3 botright = this->model_matrix() * glm::vec4(rect.botright, 1.0f);
  return collision_detection::Rectangle(topleft, botleft, botright);
}

collision_detection::Rectangle Card::raw_shape() const {
  glm::vec3  topleft(-this->width/2.0f, +this->height/2.0f, 0.0f);
  glm::vec3  botleft(-this->width/2.0f, -this->height/2.0f, 0.0f);
  glm::vec3 botright(+this->width/2.0f, -this->height/2.0f, 0.0f);
  return collision_detection::Rectangle(topleft, botleft, botright);
}
