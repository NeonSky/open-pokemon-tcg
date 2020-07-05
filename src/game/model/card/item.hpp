#pragma once

#include "card.hpp"

#include "../effects/card_effect.hpp"

#include <memory>

namespace open_pokemon_tcg::game::model {

  class ItemCard : public ICard {
  public:
    ItemCard(CardId id, CardName name, std::unique_ptr<ICardEffect> effect);
    ~ItemCard();

    CardId id() const override;
    CardName name() const override;
    std::string to_string() const override;

    ICardEffect& effect() const;

  private:
    CardId _id;
    CardId _name;
    mutable std::unique_ptr<ICardEffect> _effect;

  };

}
