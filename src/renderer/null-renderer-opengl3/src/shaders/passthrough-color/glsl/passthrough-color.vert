#version 130
uniform mat4 matrix;

in vec2 position;
in vec4 color;

out vec4 frag_color;

void main() {
	frag_color = color / 255.f;
	gl_Position = matrix * vec4(position.xy,0,1);
}