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
    void add_on_key_callback(std::function<void(GLFWwindow*, int)> callback);
    void add_on_mouse_click_callback(std::function<void(GLFWwindow*, int, int)> callback);
    void add_on_mouse_move_callback(std::function<void(GLFWwindow*, float, float)> callback);
    void add_on_mouse_scroll_callback(std::function<void(GLFWwindow*, float, float)> callback);

    // Accessors
    bool is_closing();

  private:
    GLFWwindow* window;

    std::vector<std::function<void(GLFWwindow*, int)>> on_key_callbacks;
    std::vector<std::function<void(GLFWwindow*, int, int)>> on_mouse_click_callbacks;
    std::vector<std::function<void(GLFWwindow*, float, float)>> on_mouse_move_callbacks;
    std::vector<std::function<void(GLFWwindow*, float, float)>> on_mouse_scroll_callbacks;
  };

}
