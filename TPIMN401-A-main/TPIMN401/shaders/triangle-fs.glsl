#version 460 core

in vec2 vPos;
out vec4 FragColor;

vec3 posToRGB(vec2 pos) 
{ 
    float a = 0.5*atan(pos.y,pos.x)/3.14159; 
    if (a < 0.0) 
        a += 1.0; 
    float l = length(pos); 
    vec3 rgb = clamp(abs(mod(a*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 ); 
    return mix( vec3(1.0), rgb, l); 
}

void main()
{
    vec3 rgb = posToRGB(vPos);
    FragColor = vec4(rgb, 1.0);
}
