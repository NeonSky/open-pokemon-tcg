#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  class SwitchActive : public ICardEffect {
  public:
    SwitchActive(CardEffectTarget scope);
    ~SwitchActive();

    // Mutators
    void activate(Player &self, Player &opponent, std::vector<std::reference_wrapper<const ICard>> targets) override;

    // Accessors
    std::vector<CardEffectTarget> required_targets() const override;

  private:
    CardEffectTarget _scope;

  };

}
