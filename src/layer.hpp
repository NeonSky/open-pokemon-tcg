#pragma once

namespace open_pokemon_tcg {
  class ILayer {
  public:
    virtual ~ILayer() = 0;

    void OnAttach();
    void OnDettach();
    void OnUpdate();
    void OnInteract();
  };
}
