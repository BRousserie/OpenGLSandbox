#version 410 core

#define BufferAttribPosition	0
#define BufferAttribColor		1

layout(location = BufferAttribPosition) in vec3 Position;
layout(location = BufferAttribColor) in vec4 Color;

out block
{
	vec4 Color;
} Out; 

void main()
{	
	gl_Position = vec4(Position, 1.0);
	Out.Color = Color;
}