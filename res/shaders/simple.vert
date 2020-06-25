#version 420

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoordIn;

out vec2 texCoord;

uniform mat4 modelViewProjectionMatrix;

void main() {
  gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
  texCoord = texCoordIn;
}
