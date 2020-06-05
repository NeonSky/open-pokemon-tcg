#pragma once

#include "layer.hpp"

#include <vector>

namespace open_pokemon_tcg {
  class LayerStack {
  public:
    LayerStack();
    ~LayerStack();

    // Mutators
    void push(ILayer* layer);
    void pop();
    void insert_before(int index, ILayer* layer);
    void remove(ILayer* layer);

    // Accessors
    int size();
  private:
    std::vector<ILayer*> layers;
  };
}
