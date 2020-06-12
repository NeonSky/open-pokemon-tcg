#pragma once

#include "collision_detection/collision_detection.hpp"
#include "shader.hpp"
#include "transform.hpp"

#include <array>
#include <glm/ext/scalar_constants.hpp>
#include <glm/vec3.hpp>
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

    class Intersection {
    public:
      Side side;
      AreaType area_type;
    };

    // Mutators
    virtual void render(const glm::mat4 &view_projection_matrix, Shader *shader) = 0;

    // Accessors
    virtual Intersection* does_intersect(collision_detection::Ray ray) const = 0;
    virtual Transform deck_slot(Side side) const = 0;
    virtual Transform discard_slot(Side side) const = 0;
    virtual Transform active_slot(Side side) const = 0;
    virtual Transform supporter_slot(Side side) const = 0;
    virtual Transform stadium_slot(Side side) const = 0;
    virtual std::array<Transform, 5> bench_slots(Side side) const = 0;
    virtual std::array<Transform, 6> prize_slots(Side side) const = 0;

  protected:
    static Transform mirror_transform(Transform transform);

  };
}
