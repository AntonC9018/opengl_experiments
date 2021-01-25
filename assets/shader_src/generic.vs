#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (std140) uniform Camera_View_Projection
{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;

out vec3 normal;
out vec3 position;

void main()
{
    position = vec3(model * vec4(aPos, 1.0f));
    normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}