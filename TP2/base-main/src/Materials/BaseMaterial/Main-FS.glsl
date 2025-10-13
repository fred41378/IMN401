#version 460

in vec3 vertexColor;
layout(location = 0) out vec4 Color;

void main() {

    Color = vec4(vertexColor, 1.0);
}