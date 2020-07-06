#include "effect_db.hpp"

#include "../model/effects/card_effect.hpp"
#include "../model/effects/draw.hpp"
#include "../model/effects/energy_removal.hpp"
#include "../model/effects/switch_active.hpp"

#include "../../engine/debug/logger.hpp"

#include <map>
#include <string>

using namespace open_pokemon_tcg::game;
using namespace open_pokemon_tcg::game::data;

std::unique_ptr<model::ICardEffect> CardEffectDB::load(model::CardId id) {
  // TODO: Maybe map of lambdas/functions/classrefs
  // std::map<std::string, std::function<model::ICardEffect()> db;

  if (id == "base1-91")
    return std::make_unique<model::Draw>(2);
  else if (id == "base1-92")
    return std::make_unique<model::EnergyRemoval>(model::CardEffectTarget::ENEMY_POKEMON);
  else if (id == "base1-93")
    return std::make_unique<model::SwitchActive>(model::CardEffectTarget::ENEMY_BENCH_POKEMON);
  else if (id == "base1-95")
    return std::make_unique<model::SwitchActive>(model::CardEffectTarget::FRIENDLY_BENCH_POKEMON);
  else {
    LOG_DEBUG("Could not find card effect of " + id);
    return std::make_unique<model::Draw>(0); // Dummy effect
  }
}
