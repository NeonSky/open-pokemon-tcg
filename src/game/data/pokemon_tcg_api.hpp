#pragma once

#include "../model/card/basic_energy.hpp"
#include "../model/card/card.hpp"
#include "../model/card/item.hpp"
#include "../model/card/pokemon.hpp"
#include "../model/card/stadium.hpp"
#include "../model/card/supporter.hpp"
#include "../model/card/traits/energy.hpp"
#include "../model/deck.hpp"

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

    std::unique_ptr<model::Deck> load_deck(std::string deckset_id, int deck_index = 0);
    std::unique_ptr<model::ICard> load_card(std::string card_id);

  private:
    httplib::SSLClient _api_http_client;
    httplib::SSLClient _img_http_client;

    void download_card_image(std::string card_id, std::string image_url);
    nlohmann::json download_card_data(std::string card_id, bool cache_result = true);

    std::unique_ptr<model::ICard> parse_card_data(nlohmann::json data) const;
    std::unique_ptr<model::IEnergyCard> parse_energy_card_data(nlohmann::json data) const;
    std::unique_ptr<model::PokemonCard> parse_pokemon_card_data(nlohmann::json data) const;
    std::unique_ptr<model::ItemCard> parse_item_card_data(nlohmann::json data) const;
    std::unique_ptr<model::SupporterCard> parse_supporter_card_data(nlohmann::json data) const;
    std::unique_ptr<model::StadiumCard> parse_stadium_card_data(nlohmann::json data) const;
    model::EnergyType to_energy_type(std::string name) const;

  };
}
