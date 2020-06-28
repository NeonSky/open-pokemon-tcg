#include "bench.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game::view;

Bench::Bench(model::Bench &model, std::array<engine::geometry::Rectangle, 5> slots) : _model(model), _slots(slots), _cards{} {
  _model.listen_on_place([this](model::ICard& card, unsigned int index) {
    _cards[index] = new Card(card, _slots[index].transform());
  });

  _model.listen_on_take([this](unsigned int index) {
    delete _cards[index];
  });
}

Bench::~Bench() {}

void Bench::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  for (Card* c : _cards)
    if (c != nullptr)
      c->render(view_projection_matrix, shader);
}
