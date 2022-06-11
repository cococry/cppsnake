#include "engine.hpp"
#include <assert.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GLFWwindow *engine::s_window = nullptr;
u32 engine::s_window_width = 0;
u32 engine::s_window_height = 0;
const char *engine::s_window_title;
double engine::s_delta_time = 0.0;
double engine::s_last_frame_time = 0.0;
engine *engine::s_instance = nullptr;
keyboard engine::s_keyboard;
mouse engine::s_mouse;
glm::mat4 engine::s_proj;
std::shared_ptr<shader> engine::s_snake_shader;
u32 engine::s_snake_indices[6] = {0, 1, 2, 0, 3, 2};
float engine::s_snake_vertices[8] = {0.0f, 0.0f, 0.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 0.0f};
u32 engine::s_snake_vao;
u32 engine::s_snake_vbo;
u32 engine::s_snake_ibo;
snake engine::s_snake;

engine::~engine() { shutdown(); }

void engine::initialize(const std::string &title, u32 width, u32 height) {
  assert(!s_instance);

  s_window_title = title.c_str();
  s_window_width = width;
  s_window_height = height;
  init_openGL();
  setup_window();
  setup_openGL_rendering();

  s_snake = snake(32);

  u32 centerX = ((s_window_width / 2) / s_snake.get_body_part_size());
  u32 centerY = ((s_window_height / 2) / s_snake.get_body_part_size());
  
  s_snake.add_body_part(centerX, centerY);
  s_snake.add_body_part(centerX + 1, centerY);
  s_snake.add_body_part(centerX + 2, centerY);

}
void engine::shutdown() { clean_up(); }
void engine::render() { s_snake.render(s_snake_vao, s_snake_shader, s_proj); }
void engine::update() { 
  s_snake.update_body(); 

  if(is_key_pressed(GLFW_KEY_W)) {
    s_snake.set_dir(snake_direction_up);
  }
  else if(is_key_pressed(GLFW_KEY_S)) {
    s_snake.set_dir(snake_direction_down);
  }
  else if(is_key_pressed(GLFW_KEY_D)) {
    s_snake.set_dir(snake_direction_right);
  }
  else if(is_key_pressed(GLFW_KEY_A)) {
    s_snake.set_dir(snake_direction_left);
  }
}

void engine::engine_run() {
  double time_count = 0.0;
  while (window_is_open()) {
    double this_frame_time = glfwGetTime();
    s_delta_time = this_frame_time - s_last_frame_time;
    s_last_frame_time = this_frame_time;
    time_count += s_delta_time;

    if (time_count >= ENGINE_TICK_SPEED) {
      time_count = 0.0;
      update();
    }

    glfwPollEvents();
    glfwSwapBuffers(s_window);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    render();
  }
}

void engine::clean_up() {
  glfwDestroyWindow(s_window);
  glfwTerminate();
}

void engine::setup_window() {
  assert(s_window_width);
  assert(s_window_height);

  s_window = glfwCreateWindow(s_window_width, s_window_height, s_window_title,
                              nullptr, nullptr);
  assert(s_window);

  glfwMakeContextCurrent(s_window);

  glfwSetKeyCallback(s_window, key_callback);
  glfwSetMouseButtonCallback(s_window, mouse_button_callback);
  glfwSetCursorPosCallback(s_window, mouse_cursor_pos_callback);
  glfwSetScrollCallback(s_window, mouse_scroll_callback);

  assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
}

void engine::key_callback(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
  if (action != GLFW_RELEASE) {
    if (!s_keyboard.keys[key]) {
      s_keyboard.keys[key] = true;
    }
  } else {
    s_keyboard.keys[key] = false;
  }
  s_keyboard.keys_changed[key] = (action != GLFW_REPEAT);
}
void engine::mouse_button_callback(GLFWwindow *window, int button, int action,
                                   int mods) {
  if (action != GLFW_RELEASE) {
    if (!s_mouse.buttons[button]) {
      s_mouse.buttons[button] = true;
    }
  } else {
    s_mouse.buttons[button] = false;
  }

  s_mouse.buttons_changed[button] = (action != GLFW_REPEAT);
}
void engine::mouse_cursor_pos_callback(GLFWwindow *window, double posx,
                                       double posy) {
  s_mouse.xpos = posx;
  s_mouse.ypos = posy;
}
void engine::mouse_scroll_callback(GLFWwindow *window, double xoffset,
                                   double yoffset) {
  s_mouse.x_scroll_offset = xoffset;
  s_mouse.y_scoll_offset = yoffset;
}

bool engine::is_key_pressed(int key) { return s_keyboard.keys[key]; }
bool engine::is_key_released(int key) {
  return key_changed(key) && !s_keyboard.keys[key];
}
bool engine::key_went_down(int key) {
  return key_changed(key) && s_keyboard.keys[key];
}
bool engine::key_changed(int key) {
  bool tmp = s_keyboard.keys_changed[key];
  s_keyboard.keys_changed[key] = false;
  return tmp;
}
bool engine::is_mouse_button_pressed(int button) {
  return s_mouse.buttons[button];
}
bool engine::is_mouse_button_released(int button) {
  return mouse_button_changed(button) && !s_mouse.buttons[button];
}
bool engine::mouse_button_went_down(int button) {
  return mouse_button_changed(button) && s_mouse.buttons[button];
}
bool engine::mouse_button_changed(int button) {
  bool tmp = s_mouse.buttons_changed[button];
  s_mouse.buttons_changed[button] = false;
  return tmp;
}

void engine::init_openGL() {
  assert(glfwInit());

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void engine::setup_openGL_rendering() {
  glCreateVertexArrays(1, &s_snake_vao);
  glBindVertexArray(s_snake_vao);

  glCreateBuffers(1, &s_snake_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, s_snake_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(s_snake_vertices), s_snake_vertices,
               GL_STATIC_DRAW);

  glCreateBuffers(1, &s_snake_ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_snake_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_snake_indices),
               s_snake_indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

  s_snake_shader =
      std::make_shared<shader>("../assets/shaders/vertex_color.glsl",
                               "../assets/shaders/fragment_color.glsl");
  s_proj =
      glm::ortho(0.0f, (float)s_window_width, (float)s_window_height, 0.0f);
}
