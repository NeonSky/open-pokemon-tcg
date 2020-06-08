#pragma once

#include "transform.hpp"

#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace open_pokemon_tcg {

  class IPlaymat {
  public:
    IPlaymat();
    virtual ~IPlaymat() = 0;

    enum PlayerSide {
      PLAYER1 = 0,
      PLAYER2 = 1,
    };

    // Mutators
    virtual void render(const glm::mat4 &view_projection_matrix, Shader *shader) = 0;

    // Accessors
    virtual Transform active_slot(PlayerSide side) const = 0;
    virtual Transform supporter_slot(PlayerSide side) const = 0;
    virtual Transform stadium_slot(PlayerSide side) const = 0;
    virtual std::array<Transform, 5> bench_slots(PlayerSide side) const = 0;
    virtual std::array<Transform, 6> prize_slots(PlayerSide side) const = 0;
    virtual Transform deck_slot(PlayerSide side) const = 0;
    virtual Transform discard_slot(PlayerSide side) const = 0;

  protected:
    static Transform mirror_transform(Transform transform) {
      return Transform(glm::vec3(-transform.position.x, transform.position.y, -transform.position.z),
                   transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f));
    }

  };

  IPlaymat::IPlaymat() {}
  IPlaymat::~IPlaymat() {}
}
