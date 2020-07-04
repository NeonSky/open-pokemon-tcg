#pragma once

#include "../model/card/card.hpp"
#include "../model/effects/card_effect.hpp"

#include <string>

namespace open_pokemon_tcg::game::data {

  class CardEffectDB {
  public:
    static std::unique_ptr<model::ICardEffect> load(model::CardId id);
  };
}
