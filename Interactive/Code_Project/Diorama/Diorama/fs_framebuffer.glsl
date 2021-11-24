#version 430 core

out vec4 FragColor;
in vec2 tex_coords;

uniform float exposure;
uniform sampler2D screen_texture;

const float offset_x = 1.0f / 800.0f;  
const float offset_y = 1.0f / 800.0f;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

void main()
{
    if (exposure > 0.1) {
        vec3 fragment = texture(screen_texture, tex_coords).rgb;
        vec3 color_correction = vec3(1.1, 1, 0.95);
        vec3 color = fragment * color_correction;
        vec3 tone_mapped = vec3(1.0f) - exp(-color * exposure);

        FragColor = vec4(tone_mapped, 1.0f);
    } else {
        vec3 color = vec3(0.0f);
        for(int i = 0; i < 9; i++)
            color += vec3(texture(screen_texture, tex_coords.st + offsets[i])) * kernel[i];
        FragColor = vec4(color, 1.0f);
    }
}