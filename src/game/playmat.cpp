#include "playmat.hpp"

using namespace open_pokemon_tcg;

IPlaymat::IPlaymat() {}
IPlaymat::~IPlaymat() {}

Transform IPlaymat::mirror_transform(Transform transform) {
  return Transform(glm::vec3(-transform.position.x, transform.position.y, -transform.position.z),
               transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f));
}
