#pragma once

#include "shader.hpp"

#include "defines.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

enum snake_direction {
  snake_direction_none = 0,
  snake_direction_left,
  snake_direction_right,
  snake_direction_up,
  snake_direction_down
};

class snake {
public:
  snake() = default;
  snake(u32 body_part_size);
  void add_body_part(int x, int y);

  void render(u32 vao, std::shared_ptr<shader> &shader, const glm::mat4 &proj);

  bool update_body();

  inline u32 get_body_part_size() const {return m_body_part_size;}
  inline void set_dir(snake_direction dir) {m_dir = dir;}
private:
  u32 m_body_part_size;
  
  std::vector<glm::vec2> m_body_parts;
  glm::vec2 m_snake_head;
  snake_direction m_dir = snake_direction_left;
};