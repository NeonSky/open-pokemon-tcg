#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

namespace open_pokemon_tcg::engine::gui {

  class Window {
  public:
    Window();
    Window(int width, int height, const char* title);
    ~Window();

    // Mutators
    void init_gui();
    void update();
    void clear_screen();
    void add_on_key_callback(std::function<void(GLFWwindow*)> callback);
    void add_on_cursor_callback(std::function<void(GLFWwindow*, float, float)> callback);
    void add_on_scroll_callback(std::function<void(GLFWwindow*, float, float)> callback);

    // Accessors
    bool is_closing();

  private:
    GLFWwindow* window;

    std::vector<std::function<void(GLFWwindow*)>> on_key_callbacks;
    std::vector<std::function<void(GLFWwindow*, float, float)>> on_cursor_callbacks;
    std::vector<std::function<void(GLFWwindow*, float, float)>> on_scroll_callbacks;
  };

}
