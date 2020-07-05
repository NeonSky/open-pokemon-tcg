#pragma once

#include <string>

namespace open_pokemon_tcg::game::model {

  typedef std::string CardId;
  typedef std::string CardName;

  class ICard {
  public:
    virtual CardId id() const = 0;
    virtual CardName name() const = 0;
    virtual std::string to_string() const = 0;
  };

}
