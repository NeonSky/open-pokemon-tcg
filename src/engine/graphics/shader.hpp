#pragma once

#include <glad/glad.h>
#include <string>

namespace open_pokemon_tcg::engine::graphics {

  class Shader {
  public:
    Shader(std::string vertex_shader_path, std::string fragment_shader_path);
    ~Shader();

    // Mutators
    void use();
    void set_uniform(const GLchar* uniform, const GLfloat *data);

  private:
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;

    // Accessors
    GLuint load_shader_file(std::string shader_path, GLenum shaderType);
  };
}
