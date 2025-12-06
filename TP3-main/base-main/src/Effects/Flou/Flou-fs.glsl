#version 460

layout(location = 0) out vec4 Color;

in vec2 uv;

uniform sampler2D u_src;
uniform int u_k;

void main() {
    ivec2 tSize = textureSize(u_src, 0);
    vec2 texel = 1.0 / vec2(tSize);

    vec3 sum = vec3(0.0);
    int count = 0;

    for (int j = -u_k; j <= u_k; ++j) {
        for (int i = -u_k; i <= u_k; ++i) {
            vec2 offset = vec2(float(i), float(j)) * texel;
            sum += texture(u_src, uv + offset).rgb;
            count++;
        }
    }

    Color = vec4(sum / float(count), 1.0);
}
