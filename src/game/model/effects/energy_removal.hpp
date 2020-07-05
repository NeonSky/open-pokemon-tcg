#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class EnergyRemoval : public ICardEffect {
  public:
    EnergyRemoval(CardEffectTarget pokemon_scope);
    ~EnergyRemoval();

    // Mutators
    void set_targets(std::vector<std::reference_wrapper<const ICard>> targets) override;
    void activate(Player &self, Player &opponent) override;

    // Accessors
    std::vector<CardEffectTarget> required_targets() const override;
    bool can_activate(Player &self, Player &opponent) const override;

  private:
    CardEffectTarget _pokemon_scope;
    const PokemonCard* _pokemon_target;
    const BasicEnergy* _energy_target;
  };

}
