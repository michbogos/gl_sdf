#version 330 core
layout (location = 0) in vec2 aPos;
  
out vec2 ourColor;

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    ourColor = gl_Position.xy;
}