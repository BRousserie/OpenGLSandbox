#version 410 core

#define BufferAttribVertex	0
#define BufferAttribNormal	1
#define BufferAttribColor	2

uniform mat4 MVP;
uniform mat4 MV;

layout(location = BufferAttribVertex) in vec3 Position;
layout(location = BufferAttribNormal) in vec3 Normal;
layout(location = BufferAttribColor) in vec4 Color;

out block
{
	vec4 Color;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} Out;

void main()
{	
	vec4 p = vec4(Position, 1.0);
	vec4 n = vec4(Normal, 0.0);
	gl_Position = MVP * p;
	Out.Color = Color;
	Out.CameraSpacePosition = vec3(MV * p);
	Out.CameraSpaceNormal = vec3(MV * n);
}