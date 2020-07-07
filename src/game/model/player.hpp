#pragma once

#include "playmat.hpp"
#include "hand.hpp"
#include "card/basic_energy.hpp"
#include "card/pokemon.hpp"

#include <string>
#include <vector>

namespace open_pokemon_tcg::game::model {

  class Player {
  public:
    Player(std::unique_ptr<Deck>& deck, Playmat& playmat, std::string name);
    ~Player();

    // Mutators
    void shuffle_deck_pile();
    void draw(unsigned int amount = 1);
    void mill(unsigned int amount = 1);
    void discard(const ICard& card);
    void place_on_active_slot_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card);
    void place_on_bench_from_hand(const ICard& card, unsigned int slot_index);

    void place_active_pokemon(unsigned int hand_index);
    void switch_active(const PokemonCard& pokemon_card);

    void attach_to_active_from_hand(const BasicEnergy& energy_card);
    void attach_to_bench_slot_from_hand(const BasicEnergy& energy_card, unsigned int slot_index);
    void detach_energy_from(const PokemonCard& pokemon_card, const BasicEnergy& energy_card);
    void detach_energy_from_active(const BasicEnergy& energy_card);

    void heal_pokemon(const PokemonCard& pokemon_card, unsigned int amount);

    void take_prize_card();
    void on_win(std::function<void ()> callback) const;
    void on_lose(std::function<void ()> callback) const;

    // Accessors
    void on_update_active(std::function<void (PokemonCard* card)> callback) const;

    std::string name() const;
    const Deck& deck() const;
    const Playmat& playmat() const;
    const Hand& hand() const;

  private:
    mutable engine::event::CallbackList<void ()> _on_win;
    mutable engine::event::CallbackList<void ()> _on_lose;
    mutable engine::event::CallbackList<void (PokemonCard* card)> _on_update_active;

    std::unique_ptr<Deck> _deck;
    Playmat _playmat;
    std::string _name;
    Hand _hand;
  };

}
