#version 460

uniform sampler2D myFBO;

in vec2 uv;

layout(location = 0) out vec4 Color;

void main() {
    int k = 7;
    ivec2 tSize = textureSize(myFBO, 0);
    vec2 texel = 1.0/vec2(tSize);
    vec3 c = texture(myFBO, uv).rgb;

    vec3 total = vec3(0.0);
    int compte = 0;


    for (int i = -k; i < k; ++i){
        for (int j = -k; j < k; ++j){
            vec2 voisin = vec2(float(j), float(i)) * texel;
            total += texture(myFBO, uv + voisin).rgb;
            compte++;
        }
    }

    Color = vec4(total/float(compte), 1.0);
}