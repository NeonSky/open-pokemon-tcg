#pragma once

#include "card_effect.hpp"

namespace open_pokemon_tcg::game::model::effects {

  class Draw : public ICardEffect {
  public:
    Draw(unsigned int amount);
    ~Draw();

    // Mutators
    void activate(IPokemonTrainer &self, IPokemonTrainer &opponent) override;

    // Accessors
    bool can_activate(IPokemonTrainer &self, IPokemonTrainer &opponent) override;

  private:
    unsigned int _amount;
  };

  Draw::Draw(unsigned int amount) : _amount(amount){}
  Draw::~Draw() {}

  void Draw::activate(IPokemonTrainer &self, [[maybe_unused]] IPokemonTrainer &opponent) {
    self.draw(_amount);
  }

  bool Draw::can_activate(IPokemonTrainer &self, [[maybe_unused]] IPokemonTrainer &opponent) {
    return self.deck().cards.size() >= _amount;
  }

}
