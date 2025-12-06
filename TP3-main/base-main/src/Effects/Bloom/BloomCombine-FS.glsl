#version 460
layout(location=0) out vec4 Color;

in vec2 uv;
uniform sampler2D u_src;    
uniform sampler2D u_bloom;  

uniform float u_intensite;  

void main() {
    vec3 base  = texture(u_src, uv).rgb;
    vec3 brillance  = texture(u_bloom, uv).rgb;

    vec3 outc = base + brillance * u_intensite;

    Color = vec4(outc, 1.0);
}
