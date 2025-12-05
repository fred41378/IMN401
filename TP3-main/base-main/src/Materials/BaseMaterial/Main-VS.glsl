#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal;
out vec3 vertexColor;

uniform float u_time;

void main() {

    float disp =  0.003 * sin(6.28318530718 * 0.5/1000 * u_time);
    vec3 displacedPos = Position + normalize(Normal) * disp;

    gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);

    vertexColor = abs(Normal);
}