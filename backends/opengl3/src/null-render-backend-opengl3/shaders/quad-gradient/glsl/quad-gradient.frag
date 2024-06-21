#version 330

in vec2 frag_uv;
in vec4 frag_color;

uniform vec4 color_tl;
uniform vec4 color_tr;
uniform vec4 color_br;
uniform vec4 color_bl;

out vec4 out_color;

void main() {
    vec4 premultiplied_ctl = color_tl / 255.f;
    premultiplied_ctl.rgb *= premultiplied_ctl.a;

    vec4 premultiplied_ctr = color_tr / 255.f;
    premultiplied_ctr.rgb *= premultiplied_ctr.a;

    vec4 premultiplied_cbr = color_br / 255.f;
    premultiplied_cbr.rgb *= premultiplied_cbr.a;

    vec4 premultiplied_cbl = color_bl / 255.f;
    premultiplied_cbl.rgb *= premultiplied_cbl.a;

    out_color = mix(
        mix(premultiplied_ctl, premultiplied_ctr, frag_uv.x),
        mix(premultiplied_cbl, premultiplied_cbr, frag_uv.x),
        frag_uv.y
    ) * frag_color;
}