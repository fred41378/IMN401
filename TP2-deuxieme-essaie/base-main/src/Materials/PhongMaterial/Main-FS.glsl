#version 460

layout(location = 0) out vec4 Color;
layout(location = 2) out vec3 Normal;

in vec3 vertexColor;
in vec3 Vl[4];
in vec3 Vv;
in vec3 Vn;

uniform int u_nbLumiere;
uniform float u_ka;
uniform float u_kd;
uniform float u_ks;
uniform float u_shiny;
uniform vec3 u_objColor;

void main() {

    vec3 N = normalize(Vn);
    vec3 V = normalize(Vv);

    vec3 result = u_ka * u_objColor;

    for(int i = 0; i < u_nbLumiere; ++i){
        vec3 L = normalize(Vl[i]);
        float NL = max(dot(N, L), 0);

        vec3 difus = u_kd * NL * u_objColor;

        vec3 R = reflect(-L, N);
        float RV = max(dot(R, V), 0.0);
        float specPow = pow(RV, u_shiny);
        vec3 specular = u_ks * specPow * vec3(1.0);
        
        result += difus + specular;
    }

    Color = vec4(result, 1.0);
}