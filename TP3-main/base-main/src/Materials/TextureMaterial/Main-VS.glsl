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
layout(location = 3) in vec3 Texture;
layout(location = 4) in vec4 Tangent;

out vec3 vertexColor;
out vec3 Vv;
out vec3 Vl;
out vec3 Vn;
out vec2 uv;

uniform float u_time;

//Couleurs

uniform float u_ka;
uniform float u_kd;
uniform float u_ks;
uniform float u_shiny;
uniform vec3 u_objColor;

uniform float u_id;

uniform vec3 u_posLapin;

uniform vec3 u_posLum;

uniform vec3 u_posCam;

void main() {

    uv = Texture.xy;
    vec3 pos = Position;

    vec3 L = u_posLum - pos;
    vec3 V = u_posCam - pos;

    vec3 N = normalize(Normal);
    vec3 T = normalize(Tangent.xyz);
    T = normalize(T - N * dot(N, T));
    vec3 B = cross(T,N);
    mat3 TBN = mat3(T,B,N);

    mat3 changementDeRepereT = transpose(TBN);

    Vl = changementDeRepereT * L;
    Vv = changementDeRepereT * V;
    Vn = changementDeRepereT * N;

    float distance = (length(u_posLapin - Position));

    if(distance > 2.0 && u_id == 1.0){
        float disp = 0.5 * sin(3.14 * 0.07/1000.0 * u_time);

        vec3 horizDir = vec3(Position.x, 0.0, Position.z);
        float len = length(horizDir);

        vec3 displacedPos = Position + N * disp;
        gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);
    }
    else{
        gl_Position = Proj * View * Model * vec4(Position, 1.0);
    }
}