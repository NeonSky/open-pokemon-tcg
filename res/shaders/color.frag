#version 420

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

in vec3 outColor;

layout(location = 0) out vec4 fragmentColor;

void main() {
  fragmentColor = vec4(outColor.xyz, 1.0);
}
