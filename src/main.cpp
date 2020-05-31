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

glm::vec3 camera_pos(0.0f, 0.0f, -1.0f);
glm::vec3 camera_dir(0.0f, 0.0f, -1.0f);
// glm::vec3 camera_dir(0.0f, 0.0f, 1.0f);

bool checkGLError(const char* file, int line)
{
	bool wasError = false;

	for(GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError())
    {
      wasError = true;
      // const GLubyte* sError = gluErrorString(glErr);

      // if(!sError)
      //   {
      //     sError = reinterpret_cast<const GLubyte*>(" (no message available)");
      //   }

      std::cerr << "GL Error #" << glErr << " "
                << " in File " << file << " at line: " << line << std::endl;

    }
	return wasError;
}


#define CHECK_GL_ERROR()                                      \
	{                                                          \
		checkGLError(__FILE__, __LINE__);  \
	}

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

  // std::cout << "# Vertex Shader" << std::endl;
  // std::cout << vs << std::endl;
  // std::cout << "# Fragment Shader" << std::endl;
  // std::cout << fs << std::endl;

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

void print_system_info() {
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
}

GLFWwindow* create_window(int width, int height, const char* title) {
  GLFWwindow* window;

  if (!glfwInit()) {
    std::cout << "Failed to init glfw" << std::endl;
    return nullptr;
  }

  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window) {
    std::cout << "glfw failed to create a window" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  return window;
}

GLuint create_triangle_vao(const std::vector<float> positions, const std::vector<float> uv_coords) {
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

GLuint load_texture(const char* img_path, int index) {
  int w, h, comp;
  unsigned char* image = stbi_load(img_path, &w, &h, &comp, STBI_rgb_alpha);
  if (image == NULL)
    std::cout << "Cannot load texture" << std::endl;
  else
    std::cout << "img size: " << w << " " << h << std::endl;

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, index, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  free(image);

  return texture;
}

void on_key([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods) {
  if (key == GLFW_KEY_W) {
    camera_pos += camera_dir;
    std::cout << "Forward, now at: " << glm::to_string(camera_pos) << std::endl;
  }
  if (key == GLFW_KEY_S) {
    camera_pos -= camera_dir;
    std::cout << "Backward, now at: " << glm::to_string(camera_pos) << std::endl;
  }

  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(camera_dir, worldUp));

  if (key == GLFW_KEY_D) {
    camera_pos += right;
    std::cout << "Right, now at: " << glm::to_string(camera_pos) << std::endl;
  }
  if (key == GLFW_KEY_A) {
    camera_pos -= right;
    std::cout << "Left, now at: " << glm::to_string(camera_pos) << std::endl;
  }

  if (key == GLFW_KEY_E) {
    camera_dir = glm::normalize(camera_dir + 0.1f * right);
    std::cout << "Rot Right, now looking at: " << glm::to_string(camera_dir) << std::endl;
  }

  if (key == GLFW_KEY_Q) {
    camera_dir = glm::normalize(camera_dir - 0.1f * right);
    std::cout << "Rot Right, now looking at: " << glm::to_string(camera_dir) << std::endl;
  }
}

bool firstMouse = true;
double lastX;
double lastY;
double yaw = -90.0f;
double pitch = 0.0f;

void on_mouse([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch = 89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  camera_dir = glm::normalize(direction);
}

int main() {
  auto start = std::chrono::system_clock::now();
  std::time_t start_time = std::chrono::system_clock::to_time_t(start);
  std::cout << "### Program started at: " << std::ctime(&start_time);
  // std::cout << glm::pi<float>() << std::endl;
  // std::cout << boost::dll::program_location().string() << std::endl;

  GLFWwindow *window = create_window(1920/2, 1080, "OpenPokemonTCG");
  if (window == nullptr)
    return -1;

  int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  if (!status) {
    std::cout << "Failed to init GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, on_key);
  glfwSetCursorPosCallback(window, on_mouse);

  print_system_info();

  GLuint vao = create_triangle_vao({
        //	 X      Y     Z
        0.0f,  0.0f, -1.0f, // v0
        1.0f,  0.0f, -1.0f, // v1
        1.0f,  1.0f, -1.0f  // v2
    }, {
        0.0f, 1.0f, // (u,v) for v0
        1.0f, 1.0f, // (u,v) for v1
        1.0f, 0.0f, // (u,v) for v2
  });

  GLuint vao2 = create_triangle_vao({
        //	 X      Y     Z
        1.0f,  1.0f, -1.0f, // v0
        0.0f,  1.0f, -1.0f, // v1
        0.0f,  0.0f, -1.0f  // v2
    }, {
        1.0f, 0.0f, // (u,v) for v0
        0.0f, 0.0f, // (u,v) for v1
        0.0f, 1.0f, // (u,v) for v2
    });

  GLuint vao3 = create_triangle_vao({
        //	 X      Y     Z
        0.0f,  0.0f,   -5.0f, // v0
        -1.0f,  0.0f,  -5.0f, // v1
        -1.0f,  -1.0f, -5.0f  // v2
    }, {
        1.0f, 0.0f, // (u,v) for v0
        0.0f, 0.0f, // (u,v) for v1
        0.0f, 1.0f, // (u,v) for v2
    });

  GLuint shaderProgram = loadShaderProgram("../res/shaders/simple.vert", "../res/shaders/simple.frag");
  GLuint texture = load_texture("../res/img/test.png", 0);
  // GLuint texture2 = load_texture("../res/img/cardback.png", 0);

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

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.2f, 0.8f, 1.0); // Set clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // CAMERA
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(camera_dir, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, camera_dir));
    glm::mat3 cameraBaseVectorsWorldSpace(right, up, -camera_dir);
    // transpose is the same as invese since the matrix is an orthonormal base
    glm::mat4 cameraRotation = glm::mat4(glm::transpose(cameraBaseVectorsWorldSpace));
    glm::mat4 viewMatrix = cameraRotation * glm::translate(-camera_pos);

    struct PerspectiveParams {
      float fov;
      float aspect_ratio;
      float near;
      float far;
    };
    PerspectiveParams pp = { 45.0f, 16.0f / 9.0f, 0.1f, 300.0f };

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(pp.fov), pp.aspect_ratio, pp.near, pp.far);

    glm::mat4 cardModelMatrix(1.0f);

    glm::mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * cardModelMatrix;
    // glm::mat4 modelViewProjectionMatrix = projectionMatrix * glm::translate(-camera_pos) * cardModelMatrix;

    CHECK_GL_ERROR();
    int loc = glGetUniformLocation(shaderProgram, "modelViewProjectionMatrix");
    CHECK_GL_ERROR();
    glUniformMatrix4fv(loc, 1, false, &modelViewProjectionMatrix[0].x);
    CHECK_GL_ERROR();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(vao3);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    CHECK_GL_ERROR();
    glUseProgram(0);

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

