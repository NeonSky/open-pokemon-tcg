#pragma once

#include "../playmat.hpp"

#include "../../engine/graphics/texture.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

namespace open_pokemon_tcg::playmats {

  class BlackPlaymat : public IPlaymat {
  public:
    BlackPlaymat();
    ~BlackPlaymat();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) override;

    // Accessors
    Slot* does_intersect(engine::geometry::Ray ray) const override;

    engine::geometry::Rectangle deck_slot(Side side) const override {
      const engine::geometry::Transform side1 = engine::geometry::Transform(glm::vec3(3.75f, 0.01f, -3.12f));
      const engine::geometry::Transform side2 = mirror_transform(side1);
      const engine::geometry::Transform sides[] { side1, side2 };

      engine::geometry::Transform t = sides[side] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return engine::geometry::Rectangle(t);
    }

    engine::geometry::Rectangle discard_slot(Side side) const override {
      const engine::geometry::Transform side1 = engine::geometry::Transform(glm::vec3(3.725f, 0.01f, -1.86f));
      const engine::geometry::Transform side2 = mirror_transform(side1);
      const engine::geometry::Transform sides[] { side1, side2 };

      engine::geometry::Transform t = sides[side] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return engine::geometry::Rectangle(t);
    }

    engine::geometry::Rectangle active_slot(Side side) const override {
      const engine::geometry::Transform side1 = engine::geometry::Transform(glm::vec3(-0.045f, 0.01f, -0.91f));
      const engine::geometry::Transform side2 = mirror_transform(side1);
      const engine::geometry::Transform sides[] { side1, side2 };
      engine::geometry::Transform t = sides[side] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return engine::geometry::Rectangle(t);
    }

    engine::geometry::Rectangle supporter_slot(Side side) const override {
      const engine::geometry::Transform side1 = engine::geometry::Transform(glm::vec3(1.25f, 0.01f, -0.91f));
      const engine::geometry::Transform side2 = mirror_transform(side1);
      const engine::geometry::Transform sides[] { side1, side2 };
      engine::geometry::Transform t = sides[side] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return engine::geometry::Rectangle(t);
    }

    engine::geometry::Rectangle stadium_slot(Side side) const override {
      const engine::geometry::Transform side1 = engine::geometry::Transform(glm::vec3(-2.25f, 0.01f, -1.65f));
      const engine::geometry::Transform side2 = mirror_transform(side1);
      const engine::geometry::Transform sides[] { side1, side2 };
      engine::geometry::Transform t = sides[side] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return engine::geometry::Rectangle(t);
    }

    std::array<engine::geometry::Rectangle, 5> bench_slots(Side side) const override {
      const float mid = 0.035f;
      const float spacing = 1.15f;

      const std::array<engine::geometry::Transform, 5> side1 = {
        engine::geometry::Transform(glm::vec3(mid            , 0.01f, -2.98f)),
        engine::geometry::Transform(glm::vec3(mid + spacing  , 0.01f, -2.98f)),
        engine::geometry::Transform(glm::vec3(mid + 2*spacing, 0.01f, -2.98f)),
        engine::geometry::Transform(glm::vec3(mid - spacing  , 0.01f, -2.98f)),
        engine::geometry::Transform(glm::vec3(mid - 2*spacing, 0.01f, -2.98f)),
      };
      const std::array<engine::geometry::Transform, 5> side2 = {
        mirror_transform(side1[0]),
        mirror_transform(side1[1]),
        mirror_transform(side1[2]),
        mirror_transform(side1[3]),
        mirror_transform(side1[4]),
      };
      const std::array<std::array<engine::geometry::Transform, 5>, 2> sides = { side1, side2 };

      std::array<engine::geometry::Rectangle, 5> res;
      for (unsigned long i = 0; i < res.size(); i++) {
        engine::geometry::Transform t = sides[side][i] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
        res[i] = engine::geometry::Rectangle(t);
      }

      return res;
    }

    std::array<engine::geometry::Rectangle, 6> prize_slots(Side side) const override {
      const float left = -3.59f;
      const float bot = -3.15f;
      const float spacing = 1.15f;

      const std::array<engine::geometry::Transform, 6> side1 = {
        engine::geometry::Transform(glm::vec3(left        , 0.01f , bot)),
        engine::geometry::Transform(glm::vec3(left        , 0.01f , bot + spacing)),
        engine::geometry::Transform(glm::vec3(left        , 0.01f , bot + 2.11f*spacing)),
        engine::geometry::Transform(glm::vec3(left - 0.16f, 0.005f, bot + 0.11f)),
        engine::geometry::Transform(glm::vec3(left - 0.16f, 0.005f, bot + spacing + 0.11f)),
        engine::geometry::Transform(glm::vec3(left - 0.16f, 0.005f, bot + 2.11f*spacing + 0.11f)),
      };
      const std::array<engine::geometry::Transform, 6> side2 = {
        mirror_transform(side1[0]),
        mirror_transform(side1[1]),
        mirror_transform(side1[2]),
        mirror_transform(side1[3]),
        mirror_transform(side1[4]),
        mirror_transform(side1[5]),
      };
      const std::array<std::array<engine::geometry::Transform, 6>, 2> transforms = { side1, side2 };

      std::array<engine::geometry::Rectangle, 6> res;
      for (int i = 0; i < 6; i++) {
        engine::geometry::Transform t = transforms[side][i] + engine::geometry::Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
        res[i] = engine::geometry::Rectangle(t);
      }

      return res;
    }

  private:
    static constexpr float width = 2.4f * 3.75f;
    static constexpr float height = 1.0f * 3.75f * 2.0f;

    GLuint texture;
    GLuint vao;
    engine::geometry::Transform transform;

    // Accessors
    GLuint create_vao() const;
  };

  BlackPlaymat::BlackPlaymat() : IPlaymat() {
    this->texture = engine::graphics::Texture("img/playmat2.png").id();
    this->vao = create_vao();
    this->transform = engine::geometry::Transform(glm::vec3(0.0f), glm::vec3(glm::half_pi<float>(), glm::pi<float>(), 0.0f));
  }

  BlackPlaymat::~BlackPlaymat() {}

  void BlackPlaymat::render(const glm::mat4 &view_projection_matrix, engine::graphics::Shader *shader) {
    glm::mat4 modelViewProjectionMatrix = view_projection_matrix * this->transform.matrix();
    shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
  }

  BlackPlaymat::Slot* BlackPlaymat::does_intersect(engine::geometry::Ray ray) const {
    glm::vec3 mid = active_slot(Side::PLAYER1).transform().position;
    engine::geometry::Rectangle rectangle(mid);

    auto hit = engine::geometry::ray_rectangle_intersection(ray, rectangle);
    if (hit != nullptr) {
      auto in = new BlackPlaymat::Slot();
      in->side = Side::PLAYER1;
      in->area_type = AreaType::ACTIVE_SLOT;
      in->transform = active_slot(Side::PLAYER1).transform();
      return in;
    }

    return nullptr;
  }

  GLuint BlackPlaymat::create_vao() const {
    engine::geometry::Rectangle rect(engine::geometry::Transform(), this->width, this->height);
    glm::vec3 botleft  = rect.botleft();
    glm::vec3 botright = rect.botright();
    glm::vec3 topleft  = rect.topleft();
    glm::vec3 topright = rect.topright();
    glm::vec3 midleft  = (botleft + topleft) / 2.0f;
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
