#version 430 core

out vec4 color;

in VS_OUT
{
    vec2 tc;
    vec3 normals;
    vec3 position;
    vec4 colors;
    vec4 frag_pos_light_space;
} fs_in;

uniform vec3 light_pos;
uniform mat4 camera_matrix;
uniform mat4 model_matrix;

uniform sampler2D shadow_map;

vec3 get_position(mat4 matrix)
{
	return vec3(matrix[0][3], matrix[1][3], matrix[2][3]);
}

vec3 get_front(mat4 matrix)
{
	return -vec3(matrix[0][2], matrix[1][2], matrix[2][2]);
}

float directional_light_shadows(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir, vec4 color)
{
    float shadow = 0.0;
    vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;

    proj_coords = proj_coords * 0.5 + 0.5;

    // Don't render shadow outside the texture
    if(proj_coords.z > 1.0)
        return shadow;

    float closest_depth = texture(shadow_map, proj_coords.xy).r; 
    float current_depth = proj_coords.z;
    float bias = max(0.005 * (1.0 - dot(normal, light_dir)), 0.0009); //max 0.002 min 0.0009
	int samples = 5; // Smooth factor
	vec2 pixel_size = 1.0 / textureSize(shadow_map, 0);

    // smoothing
	for(int y = -samples; y <= samples; y++)
	{
	    for(int x = -samples; x <= samples; x++)
	    {
	        float closest_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * pixel_size).r;
			if (current_depth > closest_depth + bias)
				shadow += 1.0f;
	    }    
	}
	shadow /= pow((samples * 2 + 1), 2);
    // reduce shadow intensity;
    shadow *= 0.5;
    return shadow;
}

void main(void)
{
    vec3 view_pos = get_position(camera_matrix);
    vec3 normal = normalize(fs_in.normals);
    vec3 light_color = vec3(1);
    vec3 view_dir = normalize(view_pos - fs_in.position);
    vec3 light_dir = normalize(light_pos - fs_in.position);
	float ambient = 0.50f;
	float diffuse = max(dot(normal, light_dir), 0.0f);
	float specular_light = 0.50f;
	vec3 reflection_dir = reflect(-light_dir, normal);
	float spec_amount = pow(max(dot(view_dir, reflection_dir), 0.0f), 16);
	float specular = spec_amount * specular_light;

    // non-opaque objects cannot receive shadows (eg. water)
    float shadow = (fs_in.colors.a < 0.9f) ? 0 : directional_light_shadows(fs_in.frag_pos_light_space, normal, light_dir, fs_in.colors);
    vec3 lighting = ((diffuse * (1.0f - shadow) + ambient) + specular) * (1.0f - shadow) * light_color;

    // Apply color calculations
    color = vec4(lighting, 1) * fs_in.colors;
}