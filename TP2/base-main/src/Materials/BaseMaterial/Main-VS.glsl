#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

uniform float u_time;
uniform float u_amp; 
uniform float u_freq;

uniform vec3 u_objectColor;
uniform float u_ka;
uniform float u_kd;

uniform vec3 u_posLum;

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal;

out vec3 vertexColor;

void main() {

    vec3 L = normalize(u_posLum - Position);

    float NL = dot(Normal, L);
    vec3 ambient = u_ka * u_objectColor;
    vec3 diffuse = u_kd * NL * u_objectColor;

    vertexColor = ambient + diffuse;

    float disp = u_amp * sin(6.28318530718 * u_freq/1000 * u_time);
    vec3 displacedPos = Position + normalize(Normal) * disp;

    gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);
    //vertexColor = abs(Normal);
}
