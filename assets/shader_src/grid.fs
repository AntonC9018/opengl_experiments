#version 330 core
in vec2 coord;
uniform float dim; 

void main()
{
    float parity = mod(floor(coord.x) + floor(coord.y), 2);
    gl_FragColor = vec4(parity, parity, parity, 1.0);
}