#include "pokemon_tcg_api.hpp"
#include "logger.hpp"

#include <fstream>
#include <filesystem>
#include <string>

using namespace open_pokemon_tcg;

PokemonTcgApi::PokemonTcgApi() :
  api_http_client(httplib::SSLClient("api.pokemontcg.io", 443)),
  img_http_client(httplib::SSLClient("images.pokemontcg.io", 443)) {
  std::filesystem::create_directories("res/cache/cards/data");
  std::filesystem::create_directories("res/cache/cards/img");
}

PokemonTcgApi::~PokemonTcgApi() {}

DeckModel PokemonTcgApi::load_deck(std::string deck_id) {
  std::ifstream file("res/decks/" + deck_id + ".json");
  nlohmann::json data;
  file >> data;
  return DeckModel{data};
}

CardModel PokemonTcgApi::load_card(std::string card_id) {
  nlohmann::json data;

  // Load data
  std::string data_path = "res/cache/cards/data/" + card_id + ".json";
  if (std::filesystem::exists(data_path)) {
    std::ifstream file(data_path);
    file >> data;
  } else {
    data = download_card_data(card_id);
  }

  // Load img
  std::string img_path = "res/cache/cards/img/" + card_id + ".png";
  if (!std::filesystem::exists(img_path)) {
    std::string img_url = data["card"]["imageUrlHiRes"];
    img_url = img_url.substr(28);
    download_card_image(card_id, img_url);
  }
  Texture texture("cache/cards/img/" + card_id + ".png");

  return CardModel{data, texture};
}

nlohmann::json PokemonTcgApi::download_card_data(std::string card_id, bool cache_result) {
  std::string url = "/v1/cards/" + card_id;
  auto res = this->api_http_client.Get(url.c_str());

  if (res == nullptr)
    LOG_ERROR("Fetching " + url + " resulted in a nullptr HTTP response.");
  if (res->status != 200)
    LOG_ERROR("Fetching " + url + " resulted in HTTP status code: " + std::to_string(res->status));

  nlohmann::json body = nlohmann::json::parse(res->body);
  if (cache_result) {
    std::string path = "res/cache/cards/data/" + card_id + ".json";
    std::ofstream file;
    file.open(path);
    file << body;
    file.close();
  }

  return body;
}

void PokemonTcgApi::download_card_image(std::string card_id, std::string image_url) {
  auto res = this->img_http_client.Get(image_url.c_str());

  if (res == nullptr)
    LOG_ERROR("Fetching " + image_url + " resulted in a nullptr HTTP response.");
  if (res->status != 200)
    LOG_ERROR("Fetching " + image_url + " resulted in HTTP status code: " + std::to_string(res->status));

  auto body = res->body;
  std::string path = "res/cache/cards/img/" + card_id + ".png";
  std::ofstream file;
  file.open(path);
  file << body;
  file.close();
}
