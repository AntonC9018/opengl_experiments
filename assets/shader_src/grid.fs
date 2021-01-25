#version 330 core
in vec2 initial_position;
in vec3 position;
in vec3 normal;

uniform float board_dimension; 
uniform vec3 light_position;

out vec4 FragColor;

void main()
{
    vec3 light_color = vec3(1.0);
    vec3 color = vec3(1.0);
    vec3 border = vec3(0.0);

    if (initial_position.x < 0 || initial_position.x > board_dimension || initial_position.y < 0 || initial_position.y > board_dimension)
    {
        color = vec3(border);
    }
    else
    {
        float parity = mod(floor(initial_position.x) + floor(initial_position.y), 2);
        color = vec3(parity * color);
    }

    // normal light stuff
    float ambient_factor = 0.1;
    vec3 ambient = ambient_factor * light_color;

    vec3 light_dir = normalize(light_position - position);
    float diffusion_factor = max(dot(light_dir, normalize(normal)), 0.0);
    vec3 diffuse = diffusion_factor * light_color;

    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1.0);
}