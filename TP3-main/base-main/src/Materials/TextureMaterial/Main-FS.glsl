#version 460

layout(location = 0) out vec4 Color;
layout(location = 2) out vec3 Normal;

in vec3 vertexColor;
in vec3 Vl;
in vec3 Vv;
in vec3 Vn;
in vec2 uv;

uniform int u_nbLumiere;
uniform float u_ka;
uniform float u_kd;
uniform float u_ks;
uniform float u_shiny;
uniform vec3 u_objColor;

uniform sampler2D T1;
uniform sampler2D T2;
uniform sampler2D NormalMap;

void main() {
    vec3 V = normalize(Vv);
    vec3 L = normalize(Vl);

    vec3 N = texture(NormalMap, uv).xyz * 2.0 - 1.0;
    N = normalize(N);

    vec4 texture1 = texture(T1, uv);
    vec4 texture2 = texture(T2, uv);

    vec4 textureMix = mix(texture1, texture2, clamp(0.5, 0.0, 1.0));

    vec3 albedo = textureMix.rgb * u_objColor;

    vec3 ambiant = u_ka * albedo;

    float NL = max(dot(N, L), 0);
    vec3 difus = u_kd * NL * albedo;
   
    vec3 R = reflect(-L, N);
    float RV = max(dot(R, V), 0.0);
    float specPow = pow(RV, u_shiny);
    vec3 specular = u_ks * specPow * vec3(1.0);


    vec3 result = ambiant + difus + specular;


    Color = vec4(result,1.0);
}