#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <boost/dll/runtime_symbol_info.hpp>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/string_cast.hpp> // glm::to_string

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "camera.hpp"
#include "window.hpp"
#include "shader.hpp"

using namespace open_pokemon_tcg;

Camera camera;

bool checkGLError(const char* file, int line) {
	bool wasError = false;
	for (GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError()) {
      std::cerr << "GL Error #" << glErr << " in " << file << " at line: " << line << std::endl;
      wasError = true;
    }
	return wasError;
}


#define CHECK_GL_ERROR()                                      \
	{                                                          \
		checkGLError(__FILE__, __LINE__);  \
	}

void print_system_info() {
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
}

GLuint create_vao(const std::vector<float> positions, const std::vector<float> uv_coords) {
  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);

  const float colors[] = {
    //   R     G     B
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
  };

  unsigned int color_buffer;
  glGenBuffers(1, &color_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  unsigned int uv_buffer;
  glGenBuffers(1, &uv_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv_coords.size(), uv_coords.data(), GL_STATIC_DRAW);

  GLuint vao; // Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

  // INDICES
  unsigned int index_buffer;
  const int indices[] = {
    0, 1, 2, // Triangle 1
    2, 3, 0  // Triangle 2
	};
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

	glEnableVertexAttribArray(0); // Enable the vertex position attribute
	glEnableVertexAttribArray(1); // Enable the vertex color attribute
	glEnableVertexAttribArray(2); // Enable the uv coord attribute

  return vao;
}

GLuint load_texture(std::string relative_img_path) {
  std::string res_path = boost::dll::program_location().parent_path().string() + "/../res/img/";
  std::string img_path = res_path + relative_img_path;

  int w, h, comp;
  unsigned char* image = stbi_load(img_path.c_str(), &w, &h, &comp, STBI_rgb_alpha);
  if (image == NULL)
    std::cout << "Cannot load texture" << std::endl;
  else {
    std::cout << "img path: " << img_path << std::endl;
    std::cout << "img size: " << w << " " << h << std::endl;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  free(image);

  // clamp coords
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Texture filtering
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Max samples (EXT stands for extension, and thus not from OpenGL specification)
  float anisotropy = 16.0f;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

  return texture;
}

ProjectionType proj_type = ProjectionType::PERSPECTIVE;

bool debug_mode = false;
void on_key(GLFWwindow* window, [[maybe_unused]] int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods) {

  if (glfwGetKey(window, GLFW_KEY_W) && glfwGetKey(window, GLFW_KEY_S))
    std::cout << "Forward and backward!" << std::endl;

  if (glfwGetKey(window, GLFW_KEY_W))
    camera.move(Direction::FORWARD);
  if (glfwGetKey(window, GLFW_KEY_S))
    camera.move(Direction::BACKWARD);
  if (glfwGetKey(window, GLFW_KEY_D))
    camera.move(Direction::RIGHT);
  if (glfwGetKey(window, GLFW_KEY_A))
    camera.move(Direction::LEFT);
  if (glfwGetKey(window, GLFW_KEY_E))
    camera.move(Direction::UP);
  if (glfwGetKey(window, GLFW_KEY_Q))
    camera.move(Direction::DOWN);

  if (glfwGetKey(window, GLFW_KEY_P)) {
    if (proj_type == ProjectionType::PERSPECTIVE)
      proj_type = ProjectionType::ORTHOGRAPHIC;
    else
      proj_type = ProjectionType::PERSPECTIVE;
  }

  if (glfwGetKey(window, GLFW_KEY_T)) {
    debug_mode = !debug_mode;
    if (debug_mode)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

void on_mouse([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
  if (debug_mode)
    camera.lookat_mouse(xpos, ypos);
}

float zoom = 0.0;
void on_scroll([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset) {
  zoom += (float)yoffset;
  if (zoom < 1.0f)
    zoom = 1.0f;
  if (zoom > 45.0f)
    zoom = 45.0f;
  camera.set_zoom(zoom);
}

struct Card {
  GLuint vao;
  GLuint tex;
};

#include "orientation.hpp"

Card create_card(glm::vec3 pos, Orientation orientation, std::string relative_img_path) {

  float width = 1.0f;
  float height = 1.0f;
  glm::vec3 botleft = pos - (width/2.0f) * orientation.right() - (height/2.0f) * orientation.up();
  glm::vec3 botright = pos + (width/2.0f) * orientation.right() - (height/2.0f) * orientation.up();
  glm::vec3 topleft = pos - (width/2.0f) * orientation.right() + (height/2.0f) * orientation.up();
  glm::vec3 topright = pos + (width/2.0f) * orientation.right() + (height/2.0f) * orientation.up();

  GLuint vao = create_vao({
      //	 X      Y     Z
      botleft.x, botleft.y, botleft.z,
      botright.x, botright.y, botright.z,
      topright.x, topright.y, topright.z,
      topleft.x, topleft.y, topleft.z,
    }, {
      0.0f, 1.0f, // (u,v) for v0
      1.0f, 1.0f, // (u,v) for v1
      1.0f, 0.0f, // (u,v) for v2
      0.0f, 0.0f, // (u,v) for v3
    });
  GLuint tex = load_texture(relative_img_path);
  return Card{vao, tex};
}

int main() {
  auto start = std::chrono::system_clock::now();
  std::time_t start_time = std::chrono::system_clock::to_time_t(start);
  std::cout << "### Program started at: " << std::ctime(&start_time);

  Window *window;
  try {
    window = new Window(1920/2, 1080, "OpenPokemonTCG");
  } catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  if (!status) {
    std::cout << "Failed to init GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  window->add_on_key_callback(on_key);
  window->add_on_cursor_callback(on_mouse);
  window->add_on_scroll_callback(on_scroll);

  print_system_info();

  Card card = create_card(glm::vec3(0.0f), Orientation(), "test.png");
  Card card_back = create_card(glm::vec3(0.0f), Orientation(glm::vec3(0.0f, 0.0f, 1.0f)), "cardback.png");

  Card card2 = create_card(glm::vec3(1.0f), Orientation(), "test2.png");
  Card card2_back = create_card(glm::vec3(1.0f), Orientation(glm::vec3(0.0f, 0.0f, 1.0f)), "cardback.png");

  Shader *shader = new Shader("simple.vert", "simple.frag");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glm::vec3 fwd = glm::vec3(0.0f, 0.0f, -1.0f);
  auto t = Orientation(fwd);
  camera = Camera(Orientation());

  while (!window->is_closing()) {
    window->clear_screen();

    shader->use();

    glm::mat4 cardModelMatrix(1.0f);
    glm::mat4 viewMatrix = camera.view_matrix();
    glm::mat4 projectionMatrix = camera.projection_matrix(proj_type);
    glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * cardModelMatrix;
    shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

    // CARD 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, card.tex);
    glBindVertexArray(card.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, card_back.tex);
    glBindVertexArray(card_back.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // CARD 2
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, card2.tex);
    glBindVertexArray(card2.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, card2_back.tex);
    glBindVertexArray(card2_back.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CHECK_GL_ERROR();
    glUseProgram(0);

    window->update();
  }

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Program terminated at: " << std::ctime(&end_time);
  std::cout << "Uptime: " << elapsed_seconds.count() << " seconds.\n";
}

