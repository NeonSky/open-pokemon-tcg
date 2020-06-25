#pragma once

#include "card.hpp"
#include "effects/card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  struct TrainerCardData {
    CardId id;
    CardName name;
    effects::ICardEffect *effect;
  };

  class TrainerCard : public ICard {
  public:
    TrainerCard(TrainerCardData data);
    ~TrainerCard();

    // Accessors
    effects::ICardEffect& effect();
    std::string debug() override;
    CardId id() override;
    CardName name() override;
    const TrainerCardData& data() const;

  private:
    TrainerCardData _data;

  };

}
