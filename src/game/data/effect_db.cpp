#include "effect_db.hpp"

#include "../model/effects/draw.hpp"

#include "../../engine/debug/logger.hpp"

using namespace open_pokemon_tcg::game;
using namespace open_pokemon_tcg::game::data;

std::unique_ptr<model::ICardEffect> CardEffectDB::load(model::CardId id) {
  // TODO: Maybe map of lambdas/functions/classrefs
  if (id == "base1-91") {
    return std::make_unique<model::Draw>(2);
  }
  // else if (id == "base1-92") {
  //   return std::make_unique<model::RemoveEnergy>(1, opponent);
  // }
  // else if (id == "base1-95") {
  //   return std::make_unique<model::BenchSwitch>();
  // }
  else {
    LOG_DEBUG("Could not find card effect of " + id);
    return std::make_unique<model::Draw>(0); // Dummy effect
  }
}
