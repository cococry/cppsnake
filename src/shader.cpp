#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

shader::shader(const std::string &vertex_path,
               const std::string &fragment_path) {
  u32 vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_path);
  u32 fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_path);

  link(vertex_shader, fragment_shader);
}

shader::~shader() { glDeleteProgram(m_ID); }

void shader::use() { glUseProgram(m_ID); }
void shader::unuse() { glUseProgram(0); }
void shader::delete_id() { glDeleteProgram(m_ID); }

void shader::set_mat4(const std::string &name, const glm::mat4 &value) {
  glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
void shader::set_mat3(const std::string &name, const glm::mat2 &value) {
  glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
void shader::set_mat2(const std::string &name, const glm::mat3 &value) {
  glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void shader::set_vec4(const std::string &name, const glm::vec4 &value) {
  glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1,
               glm::value_ptr(value));
}
void shader::set_vec3(const std::string &name, const glm::vec2 &value) {
  glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1,
               glm::value_ptr(value));
}
void shader::set_vec2(const std::string &name, const glm::vec3 &value) {
  glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1,
               glm::value_ptr(value));
}

void shader::set_float(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}
void shader::set_int(const std::string &name, int value) {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void shader::link(u32 vertex_shader, u32 fragment_shader) {
  int success;
  char infoLog[512];

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertex_shader);
  glAttachShader(m_ID, fragment_shader);
  glLinkProgram(m_ID);

  glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
    std::cout << "[SHADER LINKING]: Failed to link shader program:\n "
              << infoLog << "\n";
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}
u32 shader::compile_shader(GLenum type, const std::string &filepath) {
  int success;
  char infoLog[512];
  u32 shader;

  shader = glCreateShader(type);
  std::string shaderSource = get_file_content(filepath);
  const char *shaderSourceCSTR = shaderSource.c_str();

  glShaderSource(shader, 1, &shaderSourceCSTR, nullptr);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::string shaderStr = type == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
    std::cout << "[ERROR]: Failed to compile " << shaderStr << " shader:\n"
              << infoLog << "\n";
  }
  return shader;
}
std::string shader::get_file_content(const std::string &filepath) {
  std::ifstream file;
  std::stringstream ss;
  std::string returning = "";

  file.open(filepath);

  if (file.is_open()) {
    ss << file.rdbuf();
    returning = ss.str();
  } else {
    std::cout << "[FILE]: Could not open file at '" << filepath << "'\n";
  }
  return returning;
}
