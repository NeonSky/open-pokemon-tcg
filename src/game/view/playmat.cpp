#include "playmat.hpp"

using namespace open_pokemon_tcg;
using namespace open_pokemon_tcg::game::view;

IPlaymat::IPlaymat() = default;
IPlaymat::~IPlaymat() = default;

engine::geometry::Transform IPlaymat::mirror_transform(engine::geometry::Transform transform) {
  return engine::geometry::Transform(glm::vec3(-transform.position.x, transform.position.y, -transform.position.z),
               transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f));
}

std::array<engine::geometry::Rectangle, 16> IPlaymat::player_slots(Side side) const {
  auto bx = bench_slots(side);
  auto px = prize_slots(side);
  return {
      deck_slot(side),
      discard_slot(side),
      active_slot(side),
      supporter_slot(side),
      stadium_slot(side),
      bx[0], bx[1], bx[2], bx[3], bx[4],
      px[0], px[1], px[2], px[3], px[4], px[5],
  };
}
