#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class EnergyRemoval : public ICardEffect {
  public:
    EnergyRemoval(CardEffectTarget pokemon_scope);
    ~EnergyRemoval();

    // Mutators
    void activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) override;

    // Accessors
    std::vector<CardEffectTarget> required_targets() const override;

  private:
    CardEffectTarget _pokemon_scope;

  };

}
