#include "hand.hpp"

#include "../../engine/debug/logger.hpp"

#include <algorithm>

using namespace open_pokemon_tcg::game::view;


Hand::Hand(const model::Hand& model, engine::geometry::Transform transform)
  : _model(model),
    _transform(transform) {

  for (auto card : _model.cards())
    _cards.push_back(new Card(card, engine::geometry::Transform()));
  update();

  _model.listen_on_add([this](model::ICard& card) {
    _cards.push_back(new Card(card, engine::geometry::Transform()));
    update();
  });

  _model.listen_on_remove([this](int index) {
    _cards.erase(_cards.begin() + index);
    update();
  });
}

Hand::~Hand() {}

// Mutators
void Hand::update() {
  int count = _cards.size();
  if (count == 0)
    return;

  float mid = (1.0f + count) / 2.0f;
  float xspacing = std::min(0.8f, _max_width / count);
  float zspacing = 0.001f;

  for (int i = 0; i < count; i++) {
    float p = i+1;
    _cards[i]->transform.position = _transform.position + glm::vec3(xspacing * (p - mid), 0.0f, zspacing * (p - mid));
    _cards[i]->transform.rotation = _transform.rotation;
  }
}

void Hand::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (auto &c : _cards)
    c->render(view_projection_matrix, shader);
}
