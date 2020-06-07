#pragma once

#include "texture.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>

namespace open_pokemon_tcg {

  struct DeckModel {
    nlohmann::json data;
  };

  struct CardModel {
    nlohmann::json data;
    Texture texture;
  };

  // https://pokemontcg.io
  class PokemonTcgApi {
  public:
    PokemonTcgApi();
    ~PokemonTcgApi();

    DeckModel load_deck(std::string deck_id);
    CardModel load_card(std::string card_id);

  private:
    httplib::SSLClient api_http_client;
    httplib::SSLClient img_http_client;

    // Mutators
    void download_card_image(std::string card_id, std::string image_url);

    // Hybrids
    nlohmann::json download_card_data(std::string card_id, bool cache_result = true);
  };
}
