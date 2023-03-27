#version 330
uniform sampler2D sampler;

in vec2 frag_uv;
in vec4 frag_color;

uniform float aa;
uniform float outline_thickness;
uniform vec4 outline_start;
uniform vec4 outline_end;

out vec4 out_color;

float stretch(float low, float high, float x) {
    return clamp((x - low) / (high - low), 0.0, 1.0);
}

void main() {    
    float distance = texture(sampler, frag_uv.st).a;

    if (distance >= 0.5f + aa) {
        out_color = frag_color;
        return;
    }
    if(distance <= outline_thickness - aa)
        discard;

    float m = stretch(0.5f - aa, min(1.f, 0.5f + aa), distance);
    float outer_mix = stretch(outline_thickness, 0.5f, distance);
    vec4 outer = mix(outline_end / 255.f, outline_start / 255.f, outer_mix);
    outer.a *= stretch(outline_thickness - aa, outline_thickness + aa, distance);
    float ia = m * frag_color.a;
    float oa = (1.0 - m) * outer.a;
    float a = ia + oa;
    out_color = vec4((frag_color.rgb * ia + outer.rgb * oa) / a, a);
};