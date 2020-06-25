#pragma once

#include "attack.hpp"
#include "card.hpp"
#include "energy.hpp"
#include "pokemon_evolution_stage.hpp"

#include "effects/effect_target.hpp"

namespace open_pokemon_tcg::game::model {

  struct PokemonCardData {
    CardId id;
    CardName name;

    bool is_gx;
    bool is_alolan;
    EvolutionStage stage;
    CardName evolves_from;
    int hp;
    EnergyType energy_type;
    EnergyAmount retreat_cost;
    std::vector<Attack> attacks;
  };

  class PokemonCard : public ICard, public effects::IHealthTarget {
  public:
    PokemonCard(PokemonCardData data);
    ~PokemonCard();

    // Mutators
    void take_damage(unsigned int amount) override;
    void attack(unsigned int attack_index, effects::IHealthTarget &opponent);

    // Accessors
    unsigned int max_hp() const override;
    unsigned int hp() const override;

    std::string debug() override;
    CardId id() override;
    CardName name() override;
    const PokemonCardData& data() const;

  private:
    PokemonCardData _data;
    int _hp;

  };

}
