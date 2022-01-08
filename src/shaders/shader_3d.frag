#version 410 core


uniform vec3 Light;
uniform float Ambient;

layout(location = 0, index = 0) out vec4 FragColor;

in block
{
	vec4 Color;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} In; 

void main()
{
	vec3 n = normalize(In.CameraSpaceNormal);
	vec3 l = normalize(Light - In.CameraSpacePosition);
	float ndotl =  max(dot(n, l), 0.0);
	vec3 diffuse = In.Color.xyz;
	vec3 color = diffuse * ndotl + diffuse * Ambient;
	FragColor = vec4(color, In.Color.a);
}