#version 330 core
layout (location = 0) in vec3 aPos;
out vec2 coord;
uniform float dim; 

void main()
{
    gl_Position = vec4(aPos / dim - 0.5, 1.0);
    coord = aPos.xy;
}