#version 460

    layout(location=0) out vec4 Color;

in vec2 uv;
uniform sampler2D u_src;

uniform float u_limite;
uniform float u_softness;

float luminance(vec3 c) {
    return dot(c, vec3(0.2126, 0.7152, 0.0722));
}

void main() {
    vec3 c = texture(u_src, uv).rgb;
    float l = luminance(c);

    float w = smoothstep(u_limite - u_softness, u_limite + u_softness, l);

    Color = vec4(c * w, 1.0);
}
