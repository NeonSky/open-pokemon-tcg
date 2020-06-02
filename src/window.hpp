#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace open_pokemon_tcg{

  class Window {
  public:
    Window();
    Window(int width, int height, const char* title);
    ~Window();

    // Mutators
    void update();
    void clear_screen();
    void add_on_key_callback(GLFWkeyfun callback);
    void add_on_cursor_callback(GLFWcursorposfun callback);
    void add_on_scroll_callback(GLFWscrollfun cabllback);

    // Accessors
    bool is_closing();
  private:
    GLFWwindow* window;
  };

}
