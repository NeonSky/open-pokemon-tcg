#version 420

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

layout(binding = 0) uniform sampler2D tex;

in vec2 uv;

layout(location = 0) out vec4 color;

void main() {
	color = texture2D(tex, uv.xy);
}
