#pragma once

#include "defines.hpp"
#include "shader.hpp"
#include "snake.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <string>

struct keyboard {
  bool keys[GLFW_KEY_LAST];
  bool keys_changed[GLFW_KEY_LAST];
};
struct mouse {
  bool buttons[GLFW_MOUSE_BUTTON_LAST];
  bool buttons_changed[GLFW_MOUSE_BUTTON_LAST];

  double xpos, ypos;
  double x_scroll_offset, y_scoll_offset;
};
class engine {
public:
  engine() = default;
  ~engine();

  static void initialize(const std::string &title, u32 width, u32 height);
  static void shutdown();
  static void render();
  static void update();
  static void engine_run();

  static bool is_key_pressed(int key);
  static bool is_key_released(int key);
  static bool key_went_down(int key);
  static bool key_changed(int key);

  static bool is_mouse_button_pressed(int button);
  static bool is_mouse_button_released(int button);
  static bool mouse_button_went_down(int button);
  static bool mouse_button_changed(int button);

  inline static u32 get_width() { return s_window_width; }
  inline static u32 get_height() { return s_window_height; }
  inline static GLFWwindow *get_window() { return s_window; }

  inline static bool window_is_open() {
    return !glfwWindowShouldClose(s_window);
  }
  inline static void set_window_is_open(bool open) {
    glfwSetWindowShouldClose(s_window, open);
  }

  inline static double get_delta_time() { return s_delta_time; }

  inline static double get_mouse_x_pos() { return s_mouse.xpos; }
  inline static double get_mouse_y_pos() { return s_mouse.ypos; }

  inline static engine *get_instance() { return s_instance; }

private:
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                    int mods);
  static void mouse_cursor_pos_callback(GLFWwindow *window, double posx,
                                        double posy);
  static void mouse_scroll_callback(GLFWwindow *window, double xoffset,
                                    double yoffset);

  static void clean_up();

  static void setup_window();
  static void init_openGL();
  static void setup_openGL_rendering();

  static GLFWwindow *s_window;
  static u32 s_window_width;
  static u32 s_window_height;
  static const char *s_window_title;

  static double s_delta_time;
  static double s_last_frame_time;

  static engine *s_instance;

  static keyboard s_keyboard;
  static mouse s_mouse;

  static u32 s_snake_vao, s_snake_vbo, s_snake_ibo;

  static float s_snake_vertices[];
  static u32 s_snake_indices[];

  static std::shared_ptr<shader> s_snake_shader;

  static glm::mat4 s_proj;

  static snake s_snake;
};