#version 330
uniform sampler2D sampler;

in vec2 frag_uv;
in vec4 frag_color;

uniform float px_range;
uniform float glyph_rounding;
uniform float outline_rounding;
uniform float threshold;
uniform float out_bias;
uniform float outline_width_absolute;
uniform float outline_width_relative;
uniform float outline_blur;
uniform vec4 outline_color;

out vec4 out_color;

// sample code from https://github.com/Chlumsky/msdfgen
float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

//@credits: https://www.redblobgames.com/x/2404-distance-field-effects/
void main() {
    vec4 distances = texture2D(sampler, frag_uv.st);
    float msdf = median(distances.r, distances.g, distances.b);
    float sdf = distances.a;
    msdf = min(msdf, sdf + 0.1);

    float inverted_threshold = 1.0 - threshold;
    float inner = px_range * (mix(msdf, sdf, glyph_rounding) - inverted_threshold) + 0.5 + out_bias;
    float outer = px_range * (mix(msdf, sdf, outline_rounding) - inverted_threshold + outline_width_relative) + 0.5 + out_bias + outline_width_absolute;

    float inner_opacity = clamp(inner, 0.0, 1.0);
    float outer_opacity = clamp(outer, 0.0, 1.0);

    if (outline_blur > 0.0 && (outline_width_relative > 0.0 || outline_width_absolute > 0.0 )) {
        float blur_start = outline_width_relative + outline_width_absolute / px_range;
        outer_opacity *= smoothstep(blur_start,
                                    blur_start * (1.0 - outline_blur),
                                    inverted_threshold - sdf - out_bias / px_range);
    }

    vec4 premultiplied_outline_color = outline_color / 255.f;
    premultiplied_outline_color.rgb *= premultiplied_outline_color.a;

    out_color = (frag_color * inner_opacity) + (premultiplied_outline_color * (outer_opacity - inner_opacity));
}