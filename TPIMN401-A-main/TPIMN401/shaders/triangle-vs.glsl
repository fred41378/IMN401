#version 460 core

layout(location = 0) in vec2 Position;

out vec2 vPos;
uniform float u_elapsedTime;

void main()
{
    float f = 0.5;   
    float amp = 0.3; 
    float disp = amp * sin(3.14 * f * u_elapsedTime);
    float size = amp * cos(3.14 * f * u_elapsedTime);
    vec2 P = Position*(1.0+size) + vec2(disp, 0.0);

    vPos = P;
    gl_Position = vec4(P, 0.0, 1.0);
}
