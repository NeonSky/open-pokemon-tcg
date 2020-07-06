#pragma once

#include "card/pokemon.hpp"

#include "../../engine/event/event.hpp"

#include <array>
#include <functional>

namespace open_pokemon_tcg::game::model {

  class Bench {
  public:
    Bench();
    ~Bench();

    // Mutators
    void place(PokemonCard &card);
    void place(PokemonCard &card, unsigned int index);
    PokemonCard& take(unsigned int index);

    void attach_energy_to(BasicEnergy& energy_card, unsigned int slot_index);
    BasicEnergy& detach_energy_from(const PokemonCard& pokemon_card, const BasicEnergy& energy_card);

    // Accessors
    void on_place(std::function<void (PokemonCard &card, unsigned int index)> callback) const;
    void on_take(std::function<void (unsigned int index)> callback) const;

    const std::array<PokemonCard*, 5> cards() const;
    unsigned int find(const PokemonCard& card) const;

  private:
    mutable engine::event::CallbackList<void (PokemonCard&, unsigned int)> _on_place;
    mutable engine::event::CallbackList<void (unsigned int)> _on_take;

    std::array<PokemonCard*, 5> _cards;
  };
}
