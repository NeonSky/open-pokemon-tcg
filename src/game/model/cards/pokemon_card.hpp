#pragma once

#include "../card.hpp"
#include "../effects/effect_target.hpp"
#include "traits/attack.hpp"
#include "traits/energy.hpp"
#include "traits/pokemon_evolution_stage.hpp"

namespace open_pokemon_tcg::game::model::cards {

  struct PokemonCardData {
    CardId id;
    CardName name;

    bool is_gx;
    bool is_alolan;
    traits::EvolutionStage stage;
    CardName evolves_from;
    int hp;
    traits::EnergyType energy_type;
    traits::EnergyAmount retreat_cost;
    std::vector<traits::Attack> attacks;
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

    std::string debug() const override;
    CardId id() const override;
    CardName name() const override;
    const PokemonCardData& data() const;

  private:
    PokemonCardData _data;
    int _hp;

  };

}
