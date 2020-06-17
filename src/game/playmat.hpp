#pragma once

#include "../engine/geometry/collision_detection.hpp"
#include "../engine/geometry/rectangle.hpp"
#include "../engine/graphics/shader.hpp"

#include <glm/ext/scalar_constants.hpp>
#include <glm/vec3.hpp>

#include <array>
#include <vector>

namespace open_pokemon_tcg {

  class IPlaymat {
  public:
    IPlaymat();
    virtual ~IPlaymat() = 0;

    enum Side {
      PLAYER1 = 0,
      PLAYER2 = 1,
    };

    enum AreaType {
      DECK_PILE,
      DISCARD_PILE,
      ACTIVE_SLOT,
      SUPPORTER_SLOT,
      STADIUM_SLOT,
      BENCH,
      PRIZE_CARD1,
      PRIZE_CARD2,
      PRIZE_CARD3,
      PRIZE_CARD4,
      PRIZE_CARD5,
      PRIZE_CARD6,
    };

    class Slot {
    public:
      Side side;
      AreaType area_type;
      engine::geometry::Transform transform;
    };

    // Mutators
    virtual void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) = 0;

    // Accessors
    virtual Slot* does_intersect(engine::geometry::Ray ray) const = 0;
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
