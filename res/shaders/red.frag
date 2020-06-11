#version 420

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

layout(location = 0) out vec4 out_color;

void main() {
  out_color = vec4(1.0, 0.0, 0.0, 1.0);
}
