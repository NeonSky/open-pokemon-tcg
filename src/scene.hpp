#pragma once

namespace open_pokemon_tcg {

  class IScene {
  public:
    virtual ~IScene() = 0;

    virtual void update() {}
    virtual void render() {}
    virtual void gui() {}
  };

  IScene::~IScene() {}
}
