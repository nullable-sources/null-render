#version 330
uniform sampler2D sampler;

in vec2 frag_uv;
in vec4 frag_color;

uniform float amount;
uniform vec2 direction;
uniform vec2 texel_size;
uniform vec4 uv_limits;

out vec4 out_color;

const float offsets[3] = float[3](0.0f, 1.3846153846f, 3.2307692308f);
const float weights[3] = float[3](0.2270270270f, 0.3162162162f, 0.0702702703f);

vec2 clamp_uv(vec2 uv) {
    vec2 inversed = clamp(uv, uv_limits.xy, uv_limits.zw);
    return vec2(inversed.x, 1.f - inversed.y);
}

void main() {
    out_color = frag_color * texture(sampler, clamp_uv(frag_uv));
    out_color.rgb *= weights[0];

    for (int i = 1; i < 3; ++i) {
        vec2 offset = texel_size * (offsets[i] * amount) * direction;
        out_color.rgb += texture(sampler, clamp_uv(frag_uv - offset)).rgb * weights[i];
        out_color.rgb += texture(sampler, clamp_uv(frag_uv + offset)).rgb * weights[i];
    }
}