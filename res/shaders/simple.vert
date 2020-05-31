#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoordIn;

out vec3 outColor;
out vec2 texCoord;

void main() {
  // vec4 pos = vec4(position.xyz - cameraPosition.xyz, 1);
  // gl_Position = projectionMatrix * pos;
	gl_Position = vec4(position, 1.0);
  outColor = color;
  texCoord = texCoordIn;
}
