#version 460
layout(location=0) out vec4 Color;

in vec2 uv;
uniform sampler2D u_src;    
uniform sampler2D u_bloom;  

uniform float u_intensity;  

void main() {
    vec3 base  = texture(u_src, uv).rgb;
    vec3 glow  = texture(u_bloom, uv).rgb;

    vec3 outc = base + glow * u_intensity;

    Color = vec4(outc, 1.0);
}
