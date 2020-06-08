#pragma once

#include "../playmat.hpp"
#include "../texture.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

namespace open_pokemon_tcg::playmats {

  class BlackPlaymat : public IPlaymat {
  public:
    BlackPlaymat();
    ~BlackPlaymat();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, Shader *shader) override;

    // Accessors
    Transform active_slot(PlayerSide side) const override {
      const Transform transforms[] {
        Transform(transform.position + glm::vec3(-0.045f, 0.01f, -0.91f), transform.rotation),
        Transform(transform.position + glm::vec3(+0.045f, 0.01f, +0.91f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
      };
      return transforms[side];
    }

    Transform supporter_slot(PlayerSide side) const override {
      const Transform transforms[] {
        Transform(transform.position + glm::vec3(+1.25f, 0.01f, -0.91f), transform.rotation),
        Transform(transform.position + glm::vec3(-1.25f, 0.01f, +0.91f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
      };
      return transforms[side];
    }

    Transform stadium_slot(PlayerSide side) const override {
      const Transform transforms[] {
        Transform(transform.position + glm::vec3(-2.25f, 0.01f, -1.65f), transform.rotation),
        Transform(transform.position + glm::vec3(+2.25f, 0.01f, +1.65f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
      };
      return transforms[side];
    }

    std::array<Transform, 5> bench_slots(PlayerSide side) const override {
      const float mid = 0.035f;
      const float spacing = 1.15f;
      std::array<std::array<Transform, 5>, 2> transforms = {
        std::array<Transform, 5> {
          Transform(transform.position + glm::vec3(mid, 0.01f, -2.98f), transform.rotation),
          Transform(transform.position + glm::vec3(mid + spacing, 0.01f, -2.98f), transform.rotation),
          Transform(transform.position + glm::vec3(mid + 2*spacing, 0.01f, -2.98f), transform.rotation),
          Transform(transform.position + glm::vec3(mid - spacing, 0.01f, -2.98f), transform.rotation),
          Transform(transform.position + glm::vec3(mid - 2*spacing, 0.01f, -2.98f), transform.rotation),
        },
        std::array<Transform, 5> {
          Transform(transform.position + glm::vec3(mid, 0.01f, 2.98f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
          Transform(transform.position + glm::vec3(mid + spacing, 0.01f, 2.98f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
          Transform(transform.position + glm::vec3(mid + 2*spacing, 0.01f, 2.98f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
          Transform(transform.position + glm::vec3(mid - spacing, 0.01f, 2.98f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
          Transform(transform.position + glm::vec3(mid - 2*spacing, 0.01f, 2.98f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
        },
      };
      return transforms[side];
    }

    std::array<Transform, 6> prize_slots(PlayerSide side) const override {
      const float left = -3.59f;
      const float bot = -3.15f;
      const float spacing = 1.15f;

      const std::array<Transform, 6> side1 = {
        Transform(glm::vec3(left        , 0.01f , bot)),
        Transform(glm::vec3(left        , 0.01f , bot + spacing)),
        Transform(glm::vec3(left        , 0.01f , bot + 2.11f*spacing)),
        Transform(glm::vec3(left - 0.16f, 0.005f, bot + 0.11f)),
        Transform(glm::vec3(left - 0.16f, 0.005f, bot + spacing + 0.11f)),
        Transform(glm::vec3(left - 0.16f, 0.005f, bot + 2.11f*spacing + 0.11f)),
      };

      std::array<Transform, 6> side2;
      for (int i = 0; i < 6; i++)
        side2[i] = Transform(glm::vec3(-side1[i].position.x, side1[i].position.y, -side1[i].position.z),
                             glm::vec3(0.0f, glm::pi<float>(), 0.0f));

      const std::array<std::array<Transform, 6>, 2> transforms = { side1, side2 };

      std::array<Transform, 6> res;
      for (int i = 0; i < 6; i++)
        res[i] = transforms[side][i] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return res;
    }

    Transform deck_slot(PlayerSide side) const override {
      const Transform transforms[] {
        Transform(transform.position + glm::vec3(+3.75f, 0.01f, -3.12f), transform.rotation),
        Transform(transform.position + glm::vec3(-3.75f, 0.01f, +3.12f), transform.rotation + glm::vec3(0.0f, glm::pi<float>(), 0.0f)),
      };
      return transforms[side];
    }

    Transform discard_slot(PlayerSide side) const override {
      const Transform side1 = Transform(glm::vec3(+3.725f, 0.01f, -1.86f));
      const Transform side2 = mirror_transform(side1);
      const Transform transforms[] { side1, side2 };

      return transforms[side] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
    }

  private:
    GLuint texture;
    GLuint vao;
    Transform transform;

    // Accessors
    GLuint create_vao() const;
  };

  BlackPlaymat::BlackPlaymat() : IPlaymat() {
    this->texture = Texture("img/playmat2.png").id();
    this->vao = create_vao();
    this->transform = Transform();
  }

  BlackPlaymat::~BlackPlaymat() {}

  void BlackPlaymat::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
    glm::mat4 front_matrix = this->transform.matrix();
    front_matrix = glm::rotate(front_matrix, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 modelViewProjectionMatrix = view_projection_matrix * front_matrix;
    shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
  }

  GLuint BlackPlaymat::create_vao() const {
    float width = 2.4f * 3.75f;
    float height = 1.0f * 3.75f * 2.0f;
    glm::vec3 botleft = glm::vec3(-width/2.0f, -height/2.0f, 0.0f);
    glm::vec3 botright = glm::vec3(width/2.0f, -height/2.0f, 0.0f);
    glm::vec3 topleft = glm::vec3(-width/2.0f, height/2.0f, 0.0f);
    glm::vec3 topright = glm::vec3(width/2.0f, height/2.0f, 0.0f);
    glm::vec3 midleft = (botleft + topleft) / 2.0f;
    glm::vec3 midright = (botright + topright) / 2.0f;

    // x, y, z
    const float positions[] = {
      // Player 1
      botleft.x, botleft.y, botleft.z,
      botright.x, botright.y, botright.z,
      midright.x, midright.y, midright.z,
      midleft.x, midleft.y, midleft.z,

      // Player 2
      topright.x, topright.y, topright.z,
      topleft.x, topleft.y, topleft.z,
      midleft.x, midleft.y, midleft.z,
      midright.x, midright.y, midright.z,
    };

    unsigned int pos_buffer;
    glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // u, v
    const float uv_coords[] = {
      // Player 1
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,

      // Player 2
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
    };

    unsigned int uv_buffer;
    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), uv_coords, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vert1, vert2, vert3
    unsigned int index_buffer;
    const int indices[] = {
      // Player 1
      0, 1, 2,
      2, 3, 0,

      // Player 2
      4, 5, 6,
      6, 7, 4,
    };

    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

    glEnableVertexAttribArray(0); // Enable the vertex position attribute
    glEnableVertexAttribArray(2); // Enable the uv coord attribute

    return vao;
  }
}
