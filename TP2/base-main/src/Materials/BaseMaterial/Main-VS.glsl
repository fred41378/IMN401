#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

uniform float u_time;
uniform float u_amp; 
uniform float u_freq;

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal;

out vec3 vertexColor;

void main() {
    float disp = u_amp * sin(u_freq * u_time);
    vec3 displacedPos = Position + normalize(Normal) * disp;

    gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);
    vertexColor = abs(Normal);
}
