#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <boost/dll/runtime_symbol_info.hpp>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

void gui() {
}

bool linkShaderProgram(GLuint shaderProgram)
{
	glLinkProgram(shaderProgram);
	GLint linkOk = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkOk);
	if(!linkOk)
    {
      return false;
    }
	return true;
}


GLuint loadShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::ifstream vs_file(vertexShader);
	std::string vs_src((std::istreambuf_iterator<char>(vs_file)), std::istreambuf_iterator<char>());

	std::ifstream fs_file(fragmentShader);
	std::string fs_src((std::istreambuf_iterator<char>(fs_file)), std::istreambuf_iterator<char>());

	const char* vs = vs_src.c_str();
	const char* fs = fs_src.c_str();

	glShaderSource(vShader, 1, &vs, nullptr);
	glShaderSource(fShader, 1, &fs, nullptr);
	// text data is not needed beyond this point

	glCompileShader(vShader);
	int compileOk = 0;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileOk);
	if(!compileOk)
	{
		// std::string err = GetShaderInfoLog(vShader);
		// if(allow_errors)
		// {
		// 	non_fatal_error(err, "Vertex Shader");
		// }
		// else
		// {
		// 	fatal_error(err, "Vertex Shader");
		// }
		return 0;
	}

	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileOk);
	if(!compileOk)
	{
		// std::string err = GetShaderInfoLog(fShader);
		// if(allow_errors)
		// {
		// 	non_fatal_error(err, "Fragment Shader");
		// }
		// else
		// {
		// 	fatal_error(err, "Fragment Shader");
		// }
		return 0;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fShader);
	glDeleteShader(fShader);
	glAttachShader(shaderProgram, vShader);
	glDeleteShader(vShader);
	// if(!allow_errors)
	// 	CHECK_GL_ERROR();

	if(!linkShaderProgram(shaderProgram))
		return 0;

	return shaderProgram;
}

const std::string ROOT_PATH;

int main() {
  auto start = std::chrono::system_clock::now();
  std::time_t start_time = std::chrono::system_clock::to_time_t(start);
  std::cout << "### Program started at: " << std::ctime(&start_time);
  std::cout << glm::pi<float>() << std::endl;
  std::cout << boost::dll::program_location().string() << std::endl;

  GLFWwindow* window;

  if (!glfwInit())
    return -1;

  window = glfwCreateWindow(640, 480, "OpenPokemonTCG", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if(glewInit() != GLEW_OK) {
    glfwTerminate();
    return -1;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;

  // Setup 1 colored triangle.
	const float positions[] = {
    //	 X      Y     Z
    0.0f,  0.5f,  1.0f, // v0
    -0.5f, -0.5f, 1.0f, // v1
    0.5f,  -0.5f, 1.0f  // v2
	};

  unsigned int pos_buffer;
  glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
  glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

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

  GLuint vao; // Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);

	glEnableVertexAttribArray(0); // Enable the vertex position attribute
	glEnableVertexAttribArray(1); // Enable the vertex color attribute

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load text files for vertex and fragment shaders.
	std::ifstream vs_file("../res/shaders/simple.vert");
	std::string vs_src((std::istreambuf_iterator<char>(vs_file)), std::istreambuf_iterator<char>());

	std::ifstream fs_file("../res/shaders/simple.frag");
	std::string fs_src((std::istreambuf_iterator<char>(fs_file)), std::istreambuf_iterator<char>());

	const char* vs = vs_src.c_str();
	const char* fs = fs_src.c_str();

  // std::cout << "# Vertex Shader" << std::endl;
  // std::cout << vs << std::endl;
  // std::cout << "# Fragment Shader" << std::endl;
  // std::cout << fs << std::endl;

	glShaderSource(vertexShader, 1, &vs, NULL);
	glShaderSource(fragmentShader, 1, &fs, NULL);

	glCompileShader(vertexShader);
	int compileOK;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileOK);
	if(!compileOK) {
    std::cout << "Error compiling vertex shader!" << std::endl;
		// std::string err = labhelper::GetShaderInfoLog(vertexShader);
		// labhelper::fatal_error(err);
		// return;
	}

	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileOK);
	if(!compileOK) {
    std::cout << "Error compiling fragment shader!" << std::endl;
		// std::string err = labhelper::GetShaderInfoLog(fragmentShader);
		// labhelper::fatal_error(err);
		// return;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, vertexShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glLinkProgram(shaderProgram);

  // GLuint shader_program = glCreateProgram();

  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  while (!glfwWindowShouldClose(window)) {
    // glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.8f, 1.0); // Set clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glUseProgram(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "Program terminated at: " << std::ctime(&end_time);
  std::cout << "Uptime: " << elapsed_seconds.count() << " seconds.\n";
}

