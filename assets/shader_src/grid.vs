#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 initial_position;
out vec3 position;
out vec3 normal;

void main()
{
    normal = vec3(model * vec4(0.0, 0.0, 1.0, 1.0));
    position = vec3(model * vec4(aPos, 1.0));
    initial_position = aPos.xy;
    gl_Position = projection * view * vec4(position, 1.0);
}