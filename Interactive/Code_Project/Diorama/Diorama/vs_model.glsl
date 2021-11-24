#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tc;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec4 colors;

out VS_OUT
{
    vec2 tc;
    vec3 normals;
    vec3 position;
    vec4 colors;
    vec4 frag_pos_light_space;
} vs_out;

uniform vec3 wind_dir;
uniform float time;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform mat4 light_space_matrix;

void main(void)
{
    vs_out.tc = tc;
    vs_out.normals = normals;
    vs_out.position = vec3(model_matrix * vec4(position, 1.0));
    vs_out.frag_pos_light_space = light_space_matrix * vec4(vs_out.position, 1.0);
    vs_out.colors = colors;

    float waveHeight = 0.02f;
    float waveFreq = 0.5f;
    vec3 wind = wind_dir * vec3(sin(position.y * time * waveFreq) * waveHeight);

    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position + wind, 1.0);
}