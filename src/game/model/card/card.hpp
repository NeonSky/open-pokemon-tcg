#pragma once

#include <string>

namespace open_pokemon_tcg::game::model {

  typedef std::string CardId;
  typedef std::string CardName;

  enum CardType {
    POKEMON,
    ENERGY,
    ITEM,
    SUPPORTER,
    STADIUM,
  };

  class ICard {
  public:
    virtual CardId id() const = 0;
    virtual CardName name() const = 0;
    virtual CardType card_type() const = 0;
    virtual std::string to_string() const = 0;

    bool is_trainer() const {
      CardType t = card_type();
      return t == ITEM || t == SUPPORTER || t == STADIUM;
    }
  };

}
