#pragma once

#include "../card.hpp"
#include "../effects/card_effect.hpp"

namespace open_pokemon_tcg::game::model {

  struct TrainerCardData {
    CardId id;
    CardName name;
    ICardEffect *effect;
  };

  class TrainerCard : public ICard {
  public:
    TrainerCard(TrainerCardData data);
    ~TrainerCard();

    // Accessors
    ICardEffect& effect();
    std::string to_string() const override;
    CardId id() const override;
    CardName name() const override;
    const TrainerCardData& data() const;

  private:
    TrainerCardData _data;

  };

}
