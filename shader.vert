#version 450

layout(location = 0) uniform mat4 view;
layout(location = 1) in vec2 position;

void main()
{
	gl_Position = view * vec4(position, 0, 1);
}