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

out vec3 vertexColor;
out vec3 Vv;
out vec3 Vl[3];
out vec3 Vn;

uniform float u_time;

//Couleurs

uniform float u_ka;
uniform float u_kd;
uniform float u_ks;
uniform float u_shiny;
uniform vec3 u_objColor;

uniform float u_id;

uniform vec3 u_posLapin;

uniform vec3 u_posLum[3];

uniform vec3 u_posCam;

void main() {
    
    for(int i = 0; i < 3; ++i){
        Vl[i] = u_posLum[i] - Position;
    }

    Vv = u_posCam - Position;
    Vn = Normal;
    vec3 N = normalize(Normal);

    float distance = distance(u_posLapin, Position);

    if(distance > 2.0 && u_id == 1.0){
        float disp = 0.1 * sin(3.14 * 0.5/1000.0 * u_time);

        vec3 horizDir = vec3(Position.x, 0.0, Position.z);
        vec3 displacedPos = Position + N * disp;
        gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);
    }
    else{
        gl_Position = Proj * View * Model * vec4(Position, 1.0);
    }
}