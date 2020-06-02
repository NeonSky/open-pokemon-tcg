#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>
#include <cstdlib>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/string_cast.hpp> // glm::to_string

#include "camera.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "card.hpp"
#include "texture.hpp"

using namespace open_pokemon_tcg;

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

Camera camera;
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

  std::vector<Card> cards = {
    Card(glm::vec3(-1.0f), Orientation(), Texture("test1.png").id()),
    Card(glm::vec3(0.0f), Orientation(), Texture("test2.png").id()),
    Card(glm::vec3(1.0f), Orientation(), Texture("test3.png").id()),
  };

  srand(time(NULL));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 2; j++) {
      int ind = (rand() % 3) + 1;
      float x = (float) i / 2.0f;
      float z = (float) j / 2.0f;
      cards.push_back(Card(glm::vec3(x, 0.0f, z), Orientation(glm::vec3(0.01f, -1.0f, 0.0f)), Texture("test" + std::to_string(ind) + ".png").id()));
      // cards.push_back(Card(glm::vec3(i, 0.0f, j), Orientation(), Texture("test" + std::to_string(ind) + ".png").id()));
    }
  }

  Shader *shader = new Shader("simple.vert", "simple.frag");

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // camera = Camera(Orientation(glm::vec3(1.0f, 0.0f, 1.0f))); // FIXME
  camera = Camera(Orientation());

  CHECK_GL_ERROR();
  while (!window->is_closing()) {
    window->clear_screen();

    shader->use();

    glm::mat4 viewMatrix = camera.view_matrix();
    glm::mat4 projectionMatrix = camera.projection_matrix(proj_type);

    for (Card &c : cards) {
      glm::mat4 cardModelMatrix = c.model_matrix();
      glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * cardModelMatrix;
      shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

      c.render();
    }

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

