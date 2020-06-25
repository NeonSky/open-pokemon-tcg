#pragma once

#include <string>

namespace open_pokemon_tcg::game::model {

  typedef std::string CardId;
  typedef std::string CardName;

  class ICard {
  public:
    virtual std::string debug() = 0;
    virtual CardId id() = 0;
    virtual CardName name() = 0;
  };

}
