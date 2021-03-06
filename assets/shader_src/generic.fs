#version 330 core

in vec3 normal;
in vec3 position;

layout (std140) uniform Camera_View_Projection
{
    mat4 projection;
    mat4 view;
};

uniform vec3 light_position;

void main()
{
    // hardcoded for now
    vec3 light_color = vec3(1.0);
    vec3 object_color = vec3(1.0);

    float ambient_factor = 0.1;
    vec3 ambient = ambient_factor * light_color;

    vec3 light_dir = normalize(light_position - position);
    float diffusion_factor = max(dot(light_dir, normalize(normal)), 0.0);
    vec3 diffuse = diffusion_factor * light_color;

    vec3 result = (ambient + diffuse) * object_color;
    gl_FragColor = vec4(result, 1.0);
}