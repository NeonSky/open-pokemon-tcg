#version 420

uniform mat4 mvp_matrix;

layout(location = 0) in vec3 _pos;
layout(location = 1) in vec2 _uv;

out vec2 uv;

void main() {
  gl_Position = mvp_matrix * vec4(_pos, 1.0);
  uv = _uv;
}
