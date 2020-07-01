#pragma once

#include "card.hpp"

#include "../effects/card_effect.hpp"

#include <memory>

namespace open_pokemon_tcg::game::model {

  class SupporterCard : public ICard {
  public:
    SupporterCard(CardId id, CardName name, std::unique_ptr<ICardEffect> effect);
    ~SupporterCard();

    CardId id() const override;
    CardName name() const override;
    CardType card_type() const override;
    std::string to_string() const override;

  private:
    CardId _id;
    CardId _name;
    std::unique_ptr<ICardEffect> _effect;

  };

}
