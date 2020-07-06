#include "pokemon_card.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg;
using namespace open_pokemon_tcg::game::view;

PokemonCard::PokemonCard(game::model::PokemonCard &model, engine::geometry::Transform transform)
  : _model(model),
    _pokemon_view(*new Card(model, transform)),
    transform(transform) {

  for (auto& e : _model.attached_energy())
    _energy_views.push_back(new Card(e, transform));

  update_transforms();

  _model.on_energy_attached([this](const model::BasicEnergy& energy_card) {
    _energy_views.push_back(new Card(energy_card, this->transform));
    update_transforms();
  });

  _model.on_energy_detached([this](unsigned int index) {
    delete _energy_views[index];
    _energy_views.erase(_energy_views.begin() + index);
    update_transforms();
  });
}

PokemonCard::~PokemonCard() = default;

void PokemonCard::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
  _pokemon_view.render(view_projection_matrix, shader);
  for (auto& e : _energy_views)
    e->render(view_projection_matrix, shader);
}

engine::geometry::Intersection* PokemonCard::does_intersect(engine::geometry::Ray ray) const {
  return _pokemon_view.does_intersect(ray);
}

const engine::graphics::Texture& PokemonCard::texture() const {
  return _pokemon_view.texture();
}

const std::vector<Card*> PokemonCard::energy_views() const {
  return _energy_views;
}

// Privates
void PokemonCard::update_transforms() {
  glm::vec3 offset = 0.1f * (-transform.right() + -transform.up()) + (-0.001f * transform.forward());
  int i = 1;
  for (auto& e : _energy_views)
    e->transform.position = transform.position + float(i++) * offset;
}
