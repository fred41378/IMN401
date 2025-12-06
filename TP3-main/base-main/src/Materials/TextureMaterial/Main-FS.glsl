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

    uniform int   u_hasTex2;
    uniform int   u_hasNormal;

    void main() {
        vec3 V = normalize(Vv);
        vec3 L = normalize(Vl);
        vec3 N = normalize(Vn);
        if (u_hasNormal != 0) {
            N = texture(NormalMap, uv).xyz * 2.0 - 1.0;
            N = normalize(N);
        }

        vec3 texture1 = texture(T1, uv).rgb;

        vec3 textureMix = texture1;

        if (u_hasTex2 != 0) {
            vec3 texture2 = texture(T2, uv).rgb;  
            textureMix = mix(texture1, texture2, clamp(0.5, 0.0, 1.0));
        }
   
        vec3 albedo = textureMix * u_objColor;

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