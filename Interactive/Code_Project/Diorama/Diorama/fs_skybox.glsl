#version 430 core

out vec4 color;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{   
	vec3 fragment = texture(skybox, texCoords).rgb;
    vec3 color_correction = vec3(0.6, 1, 1);
    vec3 color_final = fragment * color_correction;

    color = vec4(color_final, 1.0f);
}