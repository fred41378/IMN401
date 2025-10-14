#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

uniform float u_time;
uniform float u_amp; 
uniform float u_freq;

uniform vec3 u_posLum;
uniform vec3 u_posCam;

layout(location=0) in vec3 Position;
layout(location=2) in vec3 Normal;

out vec3 vL;
out vec3 vV;
out vec3 vN;

void main(){
    vL = u_posLum - Position;
    vV = u_posCam - Position;
    vN = Normal;

    float disp = u_amp * sin(6.28318530718 * u_freq/1000 * u_time);
    vec3 displacedPos = Position + normalize(Normal) * disp;
    gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);
}
