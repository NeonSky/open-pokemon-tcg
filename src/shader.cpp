#include "shader.hpp"

#include <boost/dll/runtime_symbol_info.hpp>
#include <fstream>
#include <stdexcept>

using namespace open_pokemon_tcg;

Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path) {
  this->vertex_shader = load_shader_file(vertex_shader_path, GL_VERTEX_SHADER);
  this->fragment_shader = load_shader_file(fragment_shader_path, GL_FRAGMENT_SHADER);

  this->program = glCreateProgram();

	glAttachShader(this->program, vertex_shader);
	glDeleteShader(vertex_shader);
	glAttachShader(this->program, fragment_shader);
	glDeleteShader(fragment_shader);

	glLinkProgram(this->program);

	GLint ok = 0;
	glGetProgramiv(this->program, GL_LINK_STATUS, &ok);
	if(!ok)
    throw std::runtime_error("Could not link program.");
}

Shader::~Shader() {}

void Shader::use() {
  glUseProgram(this->program);
}

void Shader::set_uniform(const GLchar* uniform, const GLfloat *data) {
  int loc = glGetUniformLocation(this->program, uniform);
  glUniformMatrix4fv(loc, 1, false, data);
}

GLuint Shader::load_shader_file(std::string shader_path, GLenum shader_type) {
	GLuint shader = glCreateShader(shader_type);

  std::string res_path = boost::dll::program_location().parent_path().string() + "/../res/shaders/";
  std::string full_shader_path = res_path + shader_path;

	std::ifstream file(full_shader_path);
	std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	const char* raw_shader = src.c_str();
	glShaderSource(shader, 1, &raw_shader, nullptr);

	glCompileShader(shader);

	int ok = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if(!ok)
    throw std::runtime_error("Could not compile shader: " + shader_path);

  return shader;
}
