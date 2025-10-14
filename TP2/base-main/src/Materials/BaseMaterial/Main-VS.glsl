#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

uniform float u_time;
uniform float u_amp; 
uniform float u_freq;

// En rep�re OBJET
uniform vec3 u_posLum;
uniform vec3 u_posCam;

layout(location=0) in vec3 Position;
layout(location=2) in vec3 Normal;

out vec3 vL;   // non-normalis�
out vec3 vV;   // non-normalis�
out vec3 vN;   // normal non normalis�e (sera renormalis�e en FS)

void main(){
    vL = u_posLum - Position;   // vecteurs en rep�re OBJET
    vV = u_posCam - Position;
    vN = Normal;

    gl_Position = Proj * View * Model * vec4(Position, 1.0);
}
