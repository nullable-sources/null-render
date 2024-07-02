float px_range : register(c0);
float threshold : register(c1);
float out_bias : register(c2);
float outline_width_absolute : register(c3);
float outline_width_relative : register(c4);
float outline_blur : register(c5);
float4 outline_color : register(c6);

// sample code from https://github.com/Chlumsky/msdfgen
float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

//@credits: https://www.redblobgames.com/x/2404-distance-field-effects
float4 make_sdf(float sdf, float sdf_inner, float sdf_outer, float4 color) {
    float inverted_threshold = 1.0 - threshold;
    float inner = px_range * (sdf_inner - inverted_threshold) + 0.5 + out_bias;
    float outer = px_range * (sdf_outer - inverted_threshold + outline_width_relative) + 0.5 + out_bias + outline_width_absolute;

    float inner_opacity = clamp(inner, 0.0, 1.0);
    float outer_opacity = clamp(outer, 0.0, 1.0);

    if (outline_blur > 0.0 && (outline_width_relative > 0.0 || outline_width_absolute > 0.0))
    {
        float blur_start = outline_width_relative + outline_width_absolute / px_range;
        outer_opacity *= smoothstep(blur_start,
                                    blur_start * (1.0 - outline_blur),
                                    inverted_threshold - sdf - out_bias / px_range);
    }

    float4 premultiplied_outline_color = outline_color / 255.f;
    premultiplied_outline_color.rgb *= premultiplied_outline_color.a;

    return (color * inner_opacity) + (premultiplied_outline_color * (outer_opacity - inner_opacity));
}