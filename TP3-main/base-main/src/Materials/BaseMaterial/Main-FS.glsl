#version 460

layout(location = 0) out vec4 Color;
in vec3 vertexColor;
void main() {

    Color = vec4(vertexColor, 1.0);
}