#version 460
layout(location=0) out vec4 Color;

in vec2 uv;
uniform sampler2D u_src;
uniform int u_k;

void main() {
    ivec2 s = textureSize(u_src, 0);
    vec2 texel = 1.0 / vec2(s);

    vec3 sum = vec3(0.0);
    int count = 0;

    for (int i = -u_k; i <= u_k; ++i) {
        vec2 uv2 = clamp(uv + vec2(0.0, float(i) * texel.y), vec2(0.0), vec2(1.0));
        sum += texture(u_src, uv2).rgb;
        count++;
    }

    Color = vec4(sum / float(count), 1.0);
}
