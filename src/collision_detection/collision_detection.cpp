#include "collision_detection.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

using namespace open_pokemon_tcg::collision_detection;

Intersection* open_pokemon_tcg::collision_detection::ray_plane_intersection(Ray ray, Plane plane) {
  glm::vec3 from = ray.origin;
  glm::vec3 dir = ray.direction;
  glm::vec3 normal = plane.normal;

  if (glm::dot(normal, dir) == 0)
    return nullptr;

  float d0 = -glm::dot(normal, plane.point_on_plane);
  float t = -(glm::dot(normal, from) + d0) / glm::dot(normal, dir);
  glm::vec3 hit = from + t * dir;

  return new Intersection(hit);
}

Intersection* open_pokemon_tcg::collision_detection::ray_rectangle_intersection(Ray ray, Rectangle rectangle) {
  Intersection* hit = ray_plane_intersection(ray, Plane(rectangle));
  if (hit == nullptr)
    return nullptr;

  glm::vec3 topleft = rectangle.topleft;
  glm::vec3 botleft = rectangle.botleft;
  glm::vec3 botright = rectangle.botright;

  glm::vec3 OA = topleft - botleft;
  glm::vec3 OB = botright - botleft;

  bool check1 = glm::dot(botleft, OA) < glm::dot(hit->point, OA) && glm::dot(hit->point, OA) < glm::dot(topleft, OA);
  bool check2 = glm::dot(botleft, OB) < glm::dot(hit->point, OB) && glm::dot(hit->point, OB) < glm::dot(botright, OB);

  return (check1 && check2) ? hit : nullptr;
}
