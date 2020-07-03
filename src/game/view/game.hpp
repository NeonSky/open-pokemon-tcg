#include "player.hpp"
#include "playmat.hpp"

#include "../model/game.hpp"

#include <glm/mat4x4.hpp>

#include <array>
#include <memory>

namespace open_pokemon_tcg::game::view {
  class Game {
  public:
    Game(std::shared_ptr<model::Game> model, open_pokemon_tcg::game::view::IPlaymat &playmat);
    ~Game();

    // Mutators
    void render(const glm::mat4 &view_projection);

    // Accessors
    model::Game& model();
    std::array<Player*, 2> players();

  private:

    std::shared_ptr<model::Game> _model;
    open_pokemon_tcg::game::view::IPlaymat &_playmat;
    std::array<std::unique_ptr<Player>, 2> _players;

  };
}
