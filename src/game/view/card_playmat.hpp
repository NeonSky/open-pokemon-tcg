#pragma once

#include "playmat.hpp"

#include "../model/playmat.hpp"

#include <memory>

namespace open_pokemon_tcg::game::view {

  class CardPlaymat {
  public:
    CardPlaymat(std::unique_ptr<IPlaymat> playmat);
    ~CardPlaymat();

  private:
    model::Playmat _model;
    std::unique_ptr<IPlaymat> _playmat;

  };
}
