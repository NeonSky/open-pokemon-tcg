#pragma once

#include "../card.hpp"
#include "../effects/card_effect.hpp"

namespace open_pokemon_tcg::game::model::cards {

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
    std::string debug() const override;
    CardId id() const override;
    CardName name() const override;
    const TrainerCardData& data() const;

  private:
    TrainerCardData _data;

  };

}
