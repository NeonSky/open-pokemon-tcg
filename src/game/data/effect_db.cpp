#include "effect_db.hpp"

#include "../model/effects/card_effect.hpp"
#include "../model/effects/draw.hpp"
#include "../model/effects/energy_removal.hpp"
#include "../model/effects/switch_active.hpp"
#include "../model/effects/heal.hpp"

#include "../../engine/debug/logger.hpp"

#include <map>
#include <string>
#include <functional>

using namespace open_pokemon_tcg::game;
using namespace open_pokemon_tcg::game::data;

std::unique_ptr<model::ICardEffect> CardEffectDB::load(model::CardId id) {

  std::map<std::string, std::function<std::unique_ptr<model::ICardEffect> ()>> db = {
    {"base1-91", [](){ return std::make_unique<model::Draw>(2); } },
    {"base1-92", [](){ return std::make_unique<model::EnergyRemoval>(model::CardEffectTarget::ENEMY_POKEMON); } },
    {"base1-93", [](){ return std::make_unique<model::SwitchActive>(model::CardEffectTarget::ENEMY_BENCH_POKEMON); } },
    {"base1-94", [](){ return std::make_unique<model::Heal>(model::CardEffectTarget::FRIENDLY_POKEMON, 20); } },
    {"base1-95", [](){ return std::make_unique<model::SwitchActive>(model::CardEffectTarget::FRIENDLY_BENCH_POKEMON); } },
  };

  if (db.count(id))
    return db[id]();

  LOG_DEBUG("Could not find card effect of " + id);
  return std::make_unique<model::Draw>(0); // Dummy effect
}
