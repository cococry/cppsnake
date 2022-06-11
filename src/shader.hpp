#pragma once

#include "defines.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class shader {
public:
  shader() = default;
  shader(const std::string &vertex_path, const std::string &fragment_path);
  ~shader();

  void use();
  void unuse();
  void delete_id();

  void set_mat4(const std::string &name, const glm::mat4 &value);
  void set_mat3(const std::string &name, const glm::mat2 &value);
  void set_mat2(const std::string &name, const glm::mat3 &value);

  void set_vec4(const std::string &name, const glm::vec4 &value);
  void set_vec3(const std::string &name, const glm::vec2 &value);
  void set_vec2(const std::string &name, const glm::vec3 &value);

  void set_float(const std::string &name, float value);
  void set_int(const std::string &name, int value);

private:
  void link(u32 vertex_shader, u32 fragment_shader);
  u32 compile_shader(GLenum type, const std::string &filepath);
  std::string get_file_content(const std::string &filepath);

  u32 m_ID;
};
