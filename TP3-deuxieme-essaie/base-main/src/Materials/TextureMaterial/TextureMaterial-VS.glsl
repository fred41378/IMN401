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
layout(location = 3) in vec3 coodTex;
layout(location = 4) in vec4 Tangent;

out vec3 vertexColor;
out vec3 Vv;
out vec3 Vl;
out vec3 Vn;
out vec3 Vuv;

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

    vec3 N = normalize(Normal);
    vec3 L = u_posLum - Position;
    vec3 V = u_posCam - Position;
    vec3 T = normalize(Tangent.xyz);

    vec3 B = cross(N,T);

    mat3 TBN = mat3(T,B,N);
    TBN = transpose(TBN);

    Vl = TBN * L;
    Vv = TBN * V;
    Vn = TBN * N;

    Vuv = coodTex;

    gl_Position = Proj * View * Model * vec4(Position, 1.0);
}