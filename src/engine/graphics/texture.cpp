#include "texture.hpp"

#include "../debug/logger.hpp"

#include <boost/dll/runtime_symbol_info.hpp>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace open_pokemon_tcg::engine::graphics;

static std::map<std::string, GLuint> _cache;

Texture::Texture(std::string img_path) {
  if (_cache.count(img_path)) {
    _texture = _cache[img_path];
    return;
  }

  stbi_set_flip_vertically_on_load(true); // Match opengl

  std::string res_path = boost::dll::program_location().parent_path().string() + "/res/";
  std::string full_img_path = res_path + img_path;

  int w, h, comp;
  unsigned char* image = stbi_load(full_img_path.c_str(), &w, &h, &comp, STBI_rgb_alpha);
  if (image == NULL)
    LOG_ERROR("Can not load texture: " + full_img_path);

  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_2D, _texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  free(image);

  // Clamp coords
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Texture filtering
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Max samples (EXT stands for extension, and thus not from OpenGL specification)
  float anisotropy = 16.0f;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

  _cache[img_path] = _texture;
}

Texture::~Texture() {}
