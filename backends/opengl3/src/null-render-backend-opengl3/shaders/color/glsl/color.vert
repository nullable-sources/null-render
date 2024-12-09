#version 330
uniform mat4 matrix;
uniform vec2 translation;
uniform float depth;

in vec3 position;
in vec2 uv;
in vec4 color;

out vec2 frag_uv;
out vec4 frag_color;

void main() {
    frag_uv = uv;
    frag_color = color / 255.f;
    frag_color.rgb *= frag_color.a;

    gl_Position = matrix * vec4(position.xy + translation, depth, 1);
}