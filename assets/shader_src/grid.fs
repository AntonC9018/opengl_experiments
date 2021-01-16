#version 330 core
in vec3 myCoord;

void main()
{
    if (myCoord.x * myCoord.y < 0)
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}