#pragma once

namespace open_pokemon_tcg::game::model {

  enum CardEffectTarget {
    POKEMON,
    ENEMY_POKEMON,
    FRIENDLY_POKEMON,
    ENERGY,
    ENERGY_FROM_PREVIOUS_POKEMON,
    ENEMY_ENERGY,
    FRIENDLY_ENERGY,
    FRIENDLY_BENCH_POKEMON,
    ENEMY_BENCH_POKEMON,
  };

}
