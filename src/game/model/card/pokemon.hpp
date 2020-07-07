#pragma once

#include "card.hpp"
#include "basic_energy.hpp"
#include "traits/attack.hpp"
#include "traits/energy.hpp"
#include "traits/pokemon_evolution_stage.hpp"

#include "../../../engine/event/event.hpp"

#include <functional>
#include <vector>

namespace open_pokemon_tcg::game::model {

  struct PokemonCardData {
    CardId id;
    CardName name;

    bool is_gx;
    bool is_alolan;
    EvolutionStage stage;
    CardName evolves_from;
    int hp;
    EnergyType energy_type;
    EnergyAmount retreat_cost;
    std::vector<Attack*> attacks;
  };

  class PokemonCard : public ICard {
  public:
    PokemonCard(PokemonCardData data);
    ~PokemonCard();

    // Mutators
    void take_damage(unsigned int amount, EnergyType type);
    void heal(unsigned int amount);
    void attach_energy(BasicEnergy& energy_card);

    BasicEnergy& detach_energy(unsigned int index);
    BasicEnergy& detach_energy(const BasicEnergy& energy_card);

    // Accessors
    void on_energy_attached(std::function<void (const BasicEnergy& energy_card)> callback) const;
    void on_energy_detached(std::function<void (unsigned int index)> callback) const;

    std::string to_string() const override;
    CardId id() const override;
    CardName name() const override;

    unsigned int max_hp() const;
    unsigned int hp() const;

    const PokemonCardData& data() const;
    const std::vector<std::reference_wrapper<BasicEnergy>>& attached_energy() const;
    bool can_use_attack(unsigned int attack_index) const;

  private:
    mutable engine::event::CallbackList<void (const BasicEnergy& energy_card)> _on_energy_attached;
    mutable engine::event::CallbackList<void (unsigned int index)> _on_energy_detached;

    PokemonCardData _data;
    int _hp;
    std::vector<std::reference_wrapper<BasicEnergy>> _attached_energy;

  };

}
