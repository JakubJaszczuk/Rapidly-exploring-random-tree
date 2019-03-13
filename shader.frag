#version 450

layout(location = 2) uniform vec4 color;

out vec4 outColor;

void main()
{
	outColor = color;
}