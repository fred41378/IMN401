#version 460
in vec3 vL;
in vec3 vV;
in vec3 vN;

uniform vec3  u_objectColor;
uniform float u_ka;      // ambiente
uniform float u_kd;      // diffuse
uniform float u_ks;      // speculaire
uniform float u_shiny;   // Another Rare pokemon !

out vec4 FragColor;

void main(){
    vec3 N = normalize(vN);
    vec3 L = normalize(vL);
    vec3 V = normalize(vV);

    float NL = max(dot(N, L), 0.0);
    vec3 ambient = u_ka * u_objectColor;
    vec3 diffuse = u_kd * NL * u_objectColor;

    vec3 R = reflect(-L, N);
    float RV = max(dot(R, V), 0.0);
    float specPow = pow(RV, u_shiny);
    vec3 specular = u_ks * specPow * vec3(1.0);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}
