#pragma once

#include "../model/card.hpp"
#include "../model/deck.hpp"
#include "../model/cards/traits/energy.hpp"
#include "../model/cards/pokemon_card.hpp"
#include "../model/cards/trainer_card.hpp"
#include "../model/cards/energy_card.hpp"

#include "../../engine/graphics/texture.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include <string>

namespace open_pokemon_tcg::game::data {

  // https://pokemontcg.io
  class PokemonTcgApi {
  public:
    PokemonTcgApi();
    ~PokemonTcgApi();

    model::Deck load_deck(std::string deckset_id, int deck_index = 0);
    model::ICard* load_card(std::string card_id);

  private:
    httplib::SSLClient api_http_client;
    httplib::SSLClient img_http_client;

    // Mutators
    void download_card_image(std::string card_id, std::string image_url);

    // Hybrids
    nlohmann::json download_card_data(std::string card_id, bool cache_result = true);

  private:

    // Accessors
    model::ICard* parse_card_data(nlohmann::json data) const;
    model::cards::PokemonCard* parse_pokemon_card_data(nlohmann::json data) const;
    model::cards::TrainerCard* parse_trainer_card_data(nlohmann::json data) const;
    model::cards::EnergyCard* parse_energy_card_data(nlohmann::json data) const;
    model::cards::traits::EnergyType to_energy_type(std::string name) const;

  };
}
