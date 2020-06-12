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

  class GreenPlaymat : public IPlaymat {
  public:
    GreenPlaymat();
    ~GreenPlaymat();

    // Mutators
    void render(const glm::mat4 &view_projection_matrix, Shader *shader) override;

    // Accessors
    Transform deck_slot(Side side) const override {
      const Transform side1 = Transform(glm::vec3(4.305f, 0.01f, -2.330));
      const Transform side2 = mirror_transform(side1);
      const Transform sides[] { side1, side2 };
      return sides[side] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
    }

    Transform discard_slot(Side side) const override {
      const Transform side1 = Transform(glm::vec3(4.305f, 0.01f, -1.17f));
      const Transform side2 = mirror_transform(side1);
      const Transform sides[] { side1, side2 };
      return sides[side] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
    }

    Transform active_slot(Side side) const override {
      const Transform side1 = Transform(glm::vec3(0.615f, 0.01f, -1.165f));
      const Transform side2 = mirror_transform(side1);
      const Transform sides[] { side1, side2 };
      return sides[side] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
    }

    Transform supporter_slot(Side side) const override {
      const Transform side1 = Transform(glm::vec3(-0.615f, 0.01f, -1.165f));
      const Transform side2 = mirror_transform(side1);
      const Transform sides[] { side1, side2 };
      return sides[side] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
    }

    Transform stadium_slot(Side side) const override {
      const Transform side1 = Transform(glm::vec3(0.0f, 0.01f, 0.0f), glm::vec3(0.0f, glm::half_pi<float>(), 0.0f));
      const Transform side2 = mirror_transform(side1);
      const Transform sides[] { side1, side2 };
      return sides[side] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
    }

    std::array<Transform, 5> bench_slots(Side side) const override {
      const float x = 0.615f;
      const float z = -2.33f;
      const float xspacing = 1.23f;

      const std::array<Transform, 5> side1 = {
        Transform(glm::vec3(x             , 0.01f, z)),
        Transform(glm::vec3(x + xspacing  , 0.01f, z)),
        Transform(glm::vec3(x + 2*xspacing, 0.01f, z)),
        Transform(glm::vec3(x - xspacing  , 0.01f, z)),
        Transform(glm::vec3(x - 2*xspacing, 0.01f, z)),
      };
      const std::array<Transform, 5> side2 = {
        mirror_transform(side1[0]),
        mirror_transform(side1[1]),
        mirror_transform(side1[2]),
        mirror_transform(side1[3]),
        mirror_transform(side1[4]),
      };
      const std::array<std::array<Transform, 5>, 2> sides = { side1, side2 };

      std::array<Transform, 5> res;
      for (unsigned long i = 0; i < res.size(); i++)
        res[i] = sides[side][i] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));

      return res;
    }

    std::array<Transform, 6> prize_slots(Side side) const override {
      const float x = -3.075f;
      const float z = -2.330f;
      const float xspacing = 1.235;
      const float zspacing = 1.165f;

      const std::array<Transform, 6> side1 = {
        Transform(glm::vec3(x           , 0.01f, z)),
        Transform(glm::vec3(x           , 0.01f, z + zspacing)),
        Transform(glm::vec3(x           , 0.01f, z + 2*zspacing)),
        Transform(glm::vec3(x - xspacing, 0.01f, z)),
        Transform(glm::vec3(x - xspacing, 0.01f, z + zspacing)),
        Transform(glm::vec3(x - xspacing, 0.01f, z + 2*zspacing)),
      };
      const std::array<Transform, 6> side2 = {
        mirror_transform(side1[0]),
        mirror_transform(side1[1]),
        mirror_transform(side1[2]),
        mirror_transform(side1[3]),
        mirror_transform(side1[4]),
        mirror_transform(side1[5]),
      };
      const std::array<std::array<Transform, 6>, 2> sides = { side1, side2 };

      std::array<Transform, 6> res;
      for (unsigned long i = 0; i < res.size(); i++)
        res[i] = sides[side][i] + Transform(this->transform.position, this->transform.rotation, glm::vec3(0.0f));
      return res;
    }

  private:
    GLuint texture;
    GLuint vao;
    Transform transform;

    // Accessors
    GLuint create_vao() const;
  };

  GreenPlaymat::GreenPlaymat() : IPlaymat() {
    this->texture = Texture("img/playmat.png").id();
    this->vao = create_vao();
    this->transform = Transform();
  }

  GreenPlaymat::~GreenPlaymat() {}

  void GreenPlaymat::render(const glm::mat4 &view_projection_matrix, Shader *shader) {
    glm::mat4 front_matrix = this->transform.matrix();
    front_matrix = glm::rotate(front_matrix, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 modelViewProjectionMatrix = view_projection_matrix * front_matrix;
    shader->set_uniform("modelViewProjectionMatrix", &modelViewProjectionMatrix[0].x);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  GLuint GreenPlaymat::create_vao() const {
    float width = 2.4f * 4.2f;
    float height = 1.0f * 3.0f * 2.0f;
    glm::vec3 botleft = glm::vec3(-width/2.0f, -height/2.0f, 0.0f);
    glm::vec3 botright = glm::vec3(width/2.0f, -height/2.0f, 0.0f);
    glm::vec3 topleft = glm::vec3(-width/2.0f, height/2.0f, 0.0f);
    glm::vec3 topright = glm::vec3(width/2.0f, height/2.0f, 0.0f);

    // x, y, z
    const float positions[] = {
      botleft.x, botleft.y, botleft.z,
      botright.x, botright.y, botright.z,
      topright.x, topright.y, topright.z,
      topleft.x, topleft.y, topleft.z,
    };

    unsigned int pos_buffer;
    glGenBuffers(1, &pos_buffer); // Gen buffer object and store buffer id
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // u, v
    const float uv_coords[] = {
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
      0, 1, 2,
      2, 3, 0,
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
