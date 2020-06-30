#include "pokemon_tcg_api.hpp"

#include "../model/effects/draw.hpp"

#include "../../engine/debug/logger.hpp"

#include <filesystem>
#include <fstream>
#include <string>

using namespace open_pokemon_tcg::game;
using namespace open_pokemon_tcg::game::data;

PokemonTcgApi::PokemonTcgApi() :
  api_http_client(httplib::SSLClient("api.pokemontcg.io", 443)),
  img_http_client(httplib::SSLClient("images.pokemontcg.io", 443)) {
  std::filesystem::create_directories("res/cache/cards/data");
  std::filesystem::create_directories("res/cache/cards/img");
}

PokemonTcgApi::~PokemonTcgApi() {}

model::Deck PokemonTcgApi::load_deck(std::string deckset_id, int deck_index) {
  std::ifstream file("res/decks/" + deckset_id + ".json");
  nlohmann::json data;
  file >> data;

  model::Deck deck;
  deck.name = data[deck_index]["name"].get<std::string>();

  for (auto &card : data[deck_index]["cards"])
    for (int i = 0; i < card["count"]; i++)
      deck.cards.push_back(load_card(card["id"].get<std::string>()));

  return deck;
}

model::ICard* PokemonTcgApi::load_card(std::string card_id) {
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

  return parse_card_data(data["card"]);
}

model::ICard* PokemonTcgApi::parse_card_data(nlohmann::json data) const {
  std::string supertype = data["supertype"].get<std::string>();

  if (supertype == "Pokémon")
    return parse_pokemon_card_data(data);
  else if (supertype == "Trainer")
    return parse_trainer_card_data(data);
  else if (supertype == "Energy")
    return parse_energy_card_data(data);

  LOG_ERROR("Tried to load card with unknown supertype: " + supertype);
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

model::PokemonCard* PokemonTcgApi::parse_pokemon_card_data(nlohmann::json data) const {
  model::PokemonCardData card;

  card.id = data["id"];
  card.name = data["name"];

  card.is_gx = false;
  card.is_alolan = false;

  if (data["subtype"] == "Basic")
    card.stage = model::EvolutionStage::BASIC;
  else if (data["subtype"] == "Stage 1")
    card.stage = model::EvolutionStage::STAGE1;
  else if (data["subtype"] == "Stage 2")
    card.stage = model::EvolutionStage::STAGE2;
  else
    LOG_ERROR("Did not understand subtype: " + (std::string)data["subtype"]);

  if (card.stage != model::EvolutionStage::BASIC)
    card.evolves_from = data["evolvesFrom"];
  card.hp = std::stoi(data["hp"].get<std::string>());

  if (data["types"].size() > 1)
    LOG_ERROR("Currently no support for multiple types.");

  card.energy_type = to_energy_type(data["types"][0]);

  for (auto &cost : data["retreatCost"])
    card.retreat_cost += to_energy_type(cost);

  card.attacks.resize(data["attacks"].size());
  for (unsigned int i = 0; i < card.attacks.size(); i++) {
    auto attack = data["attacks"][i];

    if (attack["text"] == "") {
      model::EnergyAmount cost;
      for (auto &e : attack["cost"])
        cost += to_energy_type(e);

      // card.attacks[i] = new model::Attack(attack["name"].get<std::string>(), attack["damage"].get<int>(), cost);
    } else {
      // TODO: look up in effect db
    }
  }

  return new model::PokemonCard(card);
}

model::TrainerCard* PokemonTcgApi::parse_trainer_card_data(nlohmann::json data) const {
  model::TrainerCardData card;
  card.id = data["id"].get<std::string>();
  card.name = data["name"].get<std::string>();
  card.effect = new model::Draw(2);
  return new model::TrainerCard(card);
}

model::EnergyCard* PokemonTcgApi::parse_energy_card_data(nlohmann::json data) const {
  return new model::EnergyCard(data["id"].get<std::string>(), data["name"].get<std::string>(), model::EnergyType::FIRE);
}

model::EnergyType PokemonTcgApi::to_energy_type(std::string name) const {
  if (name == "Grass")
    return model::EnergyType::GRASS;
  else if (name == "Fire")
    return model::EnergyType::FIRE;
  else if (name == "Water")
    return model::EnergyType::WATER;
  else if (name == "Lightning")
    return model::EnergyType::LIGHTNING;
  else if (name == "Psychic")
    return model::EnergyType::PSYCHIC;
  else if (name == "Fighting")
    return model::EnergyType::FIGHTING;
  else if (name == "Darkness")
    return model::EnergyType::DARKNESS;
  else if (name == "Metal")
    return model::EnergyType::METAL;
  else if (name == "Fairy")
    return model::EnergyType::FAIRY;
  else if (name == "Dragon")
    return model::EnergyType::DRAGON;
  else if (name == "Colorless")
    return model::EnergyType::COLORLESS;
  else
    LOG_ERROR("Could not convert " + name + " to an energy type.");
}
