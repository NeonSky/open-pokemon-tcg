#pragma once

#include <glad/glad.h>
#include <string>

namespace open_pokemon_tcg::engine::graphics {

  class Texture {
  public:
    Texture(std::string img_path);
    ~Texture();

    // Accessors
    GLuint id() const { return _texture; }
  private:
    GLuint _texture;
  };
}
