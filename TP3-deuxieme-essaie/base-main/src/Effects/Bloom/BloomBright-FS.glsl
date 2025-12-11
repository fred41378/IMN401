#version 460

uniform sampler2D myFBO1;

in vec2 uv;

layout(location = 0) out vec4 Color;

void main() {
    vec3 c = texture(myFBO1, uv).rgb;

    if(length(c) > 0.5){
        Color = vec4 (c, 1.0);
    }else{
        Color = vec4(0.0);
    }
}