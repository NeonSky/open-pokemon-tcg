#pragma once

#include "../../engine/geometry/collision_detection.hpp"
#include "../../engine/geometry/rectangle.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <vector>

namespace open_pokemon_tcg::game::view {
  class IPlaymat {
  public:
    IPlaymat();
    virtual ~IPlaymat() = 0;

    enum Side {
      PLAYER1 = 0,
      PLAYER2 = 1,
    };

    // Mutators
    virtual void render(const glm::mat4 &view_projection) = 0;

    // Accessors
    std::array<engine::geometry::Rectangle, 16> player_slots(Side side) const;
    virtual engine::geometry::Rectangle deck_slot(Side side) const = 0;
    virtual engine::geometry::Rectangle discard_slot(Side side) const = 0;
    virtual engine::geometry::Rectangle active_slot(Side side) const = 0;
    virtual engine::geometry::Rectangle supporter_slot(Side side) const = 0;
    virtual engine::geometry::Rectangle stadium_slot(Side side) const = 0;
    virtual std::array<engine::geometry::Rectangle, 5> bench_slots(Side side) const = 0;
    virtual std::array<engine::geometry::Rectangle, 6> prize_slots(Side side) const = 0;

  protected:
    static engine::geometry::Transform mirror_transform(engine::geometry::Transform transform);

  };
}
