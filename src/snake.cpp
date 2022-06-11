#include "snake.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "engine.hpp"

snake::snake(u32 body_part_size) { m_body_part_size = body_part_size;}
void snake::add_body_part(int x, int y) {
  m_body_parts.emplace_back(glm::vec2(x, y));
  m_snake_head = m_body_parts[0];
}

void snake::render(u32 vao, std::shared_ptr<shader> &shader,
                   const glm::mat4 &proj) {
  for (auto body_part : m_body_parts) {
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f),
                       glm::vec3(m_body_part_size * body_part.x,
                                 m_body_part_size * body_part.y, 0.0f)) *
        glm::scale(glm::mat4(1.0f),
                   glm::vec3(m_body_part_size, m_body_part_size, 0.0f));

    shader->use();
    shader->set_mat4("u_Proj", proj);
    shader->set_mat4("u_Transform", transform);
    shader->set_vec4("u_Color", glm::vec4(RGBA_COLOR(55, 235, 52)));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }
}

bool snake::update_body() {
  auto snake_head = m_body_parts.begin();

  bool colliding = false;

  if(snake_head->x <= 0 || snake_head->x >= ((float)engine::get_width() - m_body_part_size) / m_body_part_size ||
     snake_head->y <= 0 || snake_head->y >= ((float)engine::get_height() - m_body_part_size) / m_body_part_size) {
    colliding = true;
  }

  if(!colliding) {
    glm::vec2 last_body_part(-1.0f, -1.0f);
    for(auto& body_part : m_body_parts) {
      switch (m_dir) {
        case snake_direction_up:
          if(*snake_head != body_part && snake_head->x == body_part.x && snake_head->y == body_part.y - 1) {
            colliding = true;
          }
          break; 
        case snake_direction_down:
          if(*snake_head != body_part && snake_head->x == body_part.x && snake_head->y == body_part.y + 1) {
            colliding = true;
          }
          break;
        case snake_direction_left:
          if(*snake_head != body_part && snake_head->x == body_part.x - 1 && snake_head->y == body_part.y) {
            colliding = true;
          }
          body_part.x -= 1;
          break;
        case snake_direction_right:
          if(*snake_head != body_part && snake_head->x == body_part.x + 1 && snake_head->y == body_part.y) {
            colliding = true;
          }
          break;
        default:
          break;
      }
      if(body_part != *snake_head) { 
        if(last_body_part.x == -1.0f && last_body_part.y == -1.0f) {
          body_part.x = snake_head->x;
          body_part.y = snake_head->y;

          last_body_part.x = body_part.x;
          last_body_part.y = body_part.y;
        }
        else {
          body_part.x = last_body_part.x;
          body_part.y = last_body_part.y;
        }
      }
    }
    if(!colliding) {
        switch (m_dir) {
        case snake_direction_up:
          snake_head->y -= 1;
          break;
        case snake_direction_down:
          snake_head->y += 1;
          break;
        case snake_direction_left:
          snake_head->x -= 1;
          break;
        case snake_direction_right:
          snake_head->x += 1;
          break;
        default:
          break;
        }
      }
  }
  if(colliding) {
    std::cout << "COLLDINGEG\n";
  }
  return !colliding;
}