#version 460

uniform sampler2D myFBO1;
uniform sampler2D myFBO2;

in vec2 uv;

layout(location = 0) out vec4 Color;

void main() {
    vec3 scene = texture(myFBO1, uv).rgb;
    vec3 bloom = texture(myFBO2, uv).rgb;

    vec3 color = scene + bloom * 1.5;

    Color = vec4(color, 1.0);
}